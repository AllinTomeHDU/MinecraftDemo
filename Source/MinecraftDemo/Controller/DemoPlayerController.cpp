// Developer: Jichao Luo


#include "DemoPlayerController.h"
#include "MinecraftDemo/Character/DemoCharacter.h"
#include "MinecraftPlugin/Game/MCVoxelGameState.h"
#include "MinecraftPlugin/UI/HUD/MCVoxelHUD.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryVisualizerWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCPlayerHotbarWidget.h"
#include "MinecraftPlugin/Actor/Chunk/MCChunkManager.h"
#include "MinecraftPlugin/Actor/Chunk/MCChunkBase.h"
#include "MinecraftPlugin/Inventory/MCPlayerInventory.h"
#include "MinecraftPlugin/Inventory/MCInventoryInterface.h"
#include "MinecraftPlugin/Inventory/Item/MCItem.h"
#include "MinecraftPlugin/Inventory/Item/MCItemDatabase.h"
#include "MinecraftPlugin/Library/MCVoxelUtilsLibrary.h"
#include "MotionMatchingAls/3C/Character/MMAlsCharacter.h"
#include "MotionMatchingAls/3C/Camera/MMAlsCameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


ADemoPlayerController::ADemoPlayerController()
{

}

void ADemoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComp)
	{
		EnhancedInputComp->BindAction(IA_LeftMouse, ETriggerEvent::Triggered, this, &ThisClass::LeftMouseAction);
		EnhancedInputComp->BindAction(IA_RightMouse, ETriggerEvent::Started, this, &ThisClass::RightMouseAction);
		EnhancedInputComp->BindAction(IA_Inventory, ETriggerEvent::Started, this, &ThisClass::InventoryActoin);
		EnhancedInputComp->BindAction(IA_HotbarCursor, ETriggerEvent::Started, this, &ThisClass::HotbarCursorActrion);
		EnhancedInputComp->BindAction(IA_CameraMode, ETriggerEvent::Started, this, &ThisClass::ToggleViewModeAction);
	}

	auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(IMC_MCDemo, MCDemoInputsPriority);
	}
}

void ADemoPlayerController::LeftMouseAction()
{
	OnLeftMouseTrigger.Broadcast();
}

void ADemoPlayerController::RightMouseAction()
{
	int32 ViewportX, ViewportY;
	GetViewportSize(ViewportX, ViewportY);
	FVector2D ScreenCenter(ViewportX / 2.f, ViewportY / 2.f);

	FVector WorldOrigin, WorldDirection;
	if (UGameplayStatics::DeprojectScreenToWorld(this, ScreenCenter, WorldOrigin, WorldDirection))
	{
		FVector Start = WorldOrigin;
		FVector End = Start + (WorldDirection * 2000.f);

		FHitResult HitResult;
		bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			{},
			EDrawDebugTrace::None,
			HitResult,
			true
		);
		if (!bHit) return;

		if (HitResult.GetActor()->ActorHasTag(AMCChunkManager::DefaultChunkTag))
		{
			if (FVector::Dist(HitResult.ImpactPoint, Chr->GetActorLocation()) < 500.f)
			{
				auto GS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState());
				AMCChunkBase* HitChunk = Cast<AMCChunkBase>(HitResult.GetActor());
				FVector WorldPosition = HitResult.Location - HitResult.Normal;
				FIntVector Coord = UMCVoxelUtilsLibrary::WorldToLocalBlockPosition(WorldPosition, HitChunk->GetChunkSize());
				switch (HitChunk->GetBlock(Coord))
				{
					case EMCBlock::Inventory:
					{
						TScriptInterface<IMCInventoryInterface> WorldInventory;
						if (GS->GetInventoryDatabase().GetWorldInventory(UMCVoxelUtilsLibrary::WorldToBlockPosition(WorldPosition), WorldInventory))
						{
							bool bIsInventoryDisplayed;
							Cast<AMCVoxelHUD>(GetHUD())->GetInventoryVisualizer()->ToggleBothInventories(
								GS->GetPlayerInventory(),
								WorldInventory,
								bIsInventoryDisplayed
							);
							UpdateInteractInputMode(bIsInventoryDisplayed);
						}
						break;
					}
					default:
					{
						UMCItem* RemovedItem;
						auto PlayerInventory = GS->GetPlayerInventory();
						if (PlayerInventory->RemoveNumberFrom(PlayerInventory->GetCurrentlySelectedHotbarSlotCoords(), 1, RemovedItem))
						{
							auto Block = GS->GetItemInfoDatabase()->GetItemInfo(RemovedItem->GetAssociatedItemID()).BlockType;
							PlaceBlock(HitChunk, HitResult.Location - HitResult.Normal, HitResult.Normal, Block);
							Cast<AMCVoxelHUD>(GetHUD())->GetPlayerHotbar()->UpdateItems();
						}
						break;
					}
				}
			}
		}
	}
}

void ADemoPlayerController::PlaceBlock(AMCChunkBase* Chunk, const FVector& WorldPos, const FVector& HitNormal, EMCBlock Block)
{
	auto GS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState());
	switch (Block)
	{
	case EMCBlock::Inventory:
		GS->GetInventoryDatabase().AddWorldInventory(
			UMCVoxelUtilsLibrary::WorldToBlockPosition(WorldPos) + FIntVector(HitNormal),
			NewObject<UMCInventory>()
		);
		// goto default
	default:
		Chunk->ModifyVoxel(
			UMCVoxelUtilsLibrary::WorldToLocalBlockPosition(WorldPos, Chunk->GetChunkSize()) + FIntVector(HitNormal), 
			Block
		);
		break;
	}
}

void ADemoPlayerController::InventoryActoin()
{
	auto VoxelHUD = Cast<AMCVoxelHUD>(GetHUD());
	auto VoxelGS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState());
	if (!IsValid(VoxelHUD) || !IsValid(VoxelGS)) return;

	bool bIsDisplayed;
	VoxelHUD->GetInventoryVisualizer()->TogglePlayerInventory(VoxelGS->GetPlayerInventory(), bIsDisplayed);

	UpdateInteractInputMode(bIsDisplayed);
}

void ADemoPlayerController::UpdateInteractInputMode(const bool bIsDisplayed)
{
	if (bIsDisplayed)
	{
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}
}

void ADemoPlayerController::HotbarCursorActrion(const FInputActionValue& Value)
{
	// 往上滚动是正值，往下滚动是负值
	auto GS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState());
	auto HUD = Cast<AMCVoxelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(GS) || !IsValid(HUD)) return;

	int32 HotbarSize = GS->GetPlayerInventory()->GetHotbarSize().X;
	int32 CurrentSlot = GS->GetPlayerInventory()->GetCurrentlySelectedHotbarSlot();
	int32 TargetSlot;
	if (Value.Get<float>() > 0)
	{
		TargetSlot = CurrentSlot + 1 > HotbarSize - 1 ? 0 : CurrentSlot + 1;  // Next
	}
	else
	{
		TargetSlot = CurrentSlot - 1 < 0 ? HotbarSize - 1 : CurrentSlot - 1;  // Prev
	}
	GS->GetPlayerInventory()->SetCurrentlySelectedHotbarSlot(TargetSlot);
	HUD->GetPlayerHotbar()->UpdateCursorPosition();
}

void ADemoPlayerController::ToggleViewModeAction()
{
	if (auto Camera = (Chr->GetCameraComponent()))
	{
		EMMAlsViewMode ViewMode = Camera->GetViewMode() == EMMAlsViewMode::FirstPerson ?
			EMMAlsViewMode::ThirdPerson : EMMAlsViewMode::FirstPerson;
		Camera->SetViewMode(ViewMode);
		if (Cast<ADemoCharacter>(Chr))
		{
			if (ViewMode == EMMAlsViewMode::FirstPerson)
			{
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(
					TimerHandle,
					[&]() { Cast<ADemoCharacter>(Chr)->UpdateMeshesRenderingMode(); },
					0.4f,
					false
				);
			}
			else
			{
				Cast<ADemoCharacter>(Chr)->UpdateMeshesRenderingMode();
			}
		}
	}
}

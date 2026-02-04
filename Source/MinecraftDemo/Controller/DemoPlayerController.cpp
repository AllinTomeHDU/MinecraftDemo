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
#include "Room/MultiplayerRoomSubsystem.h"
#include "GameFramework/PlayerStart.h"
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

void ADemoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);

		if (auto RoomSubsystem = GetGameInstance()->GetSubsystem<UMultiplayerRoomSubsystem>())
		{
			if (UClass* InClass = RoomSubsystem->GetSavedCharacterClass())
			{
				Server_SpawnHeroCharacter(InClass);
			}
			if (!GetCharacter() && IsValid(DefaultCharacter))
			{
				Server_SpawnHeroCharacter(DefaultCharacter);
			}
		}
		else
		{
			if (!GetCharacter() && IsValid(DefaultCharacter))
			{
				Server_SpawnHeroCharacter(DefaultCharacter);
			}
		}

		if (auto DemoChr = Cast<ADemoCharacter>(GetPawn()))
		{
			DemoChr->SetHandsObjectVisibility(true);
			DemoChr->SetBodyOwnerNoSee(true);
		}
	}
}

void ADemoPlayerController::Server_SpawnHeroCharacter_Implementation(UClass* InClass)
{
	UWorld* World = GetWorld();

	// 为避免角色生成时位置冲突，可依据玩家数摆放出生点
	AActor* StartPoint = nullptr;
	TArray<AActor*> FoundStarts;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), FoundStarts);
	checkf(FoundStarts.Num() > 0, TEXT("Can't found player start point..."));
	for (auto& Tmp : FoundStarts)
	{
		if (!Tmp->ActorHasTag(TEXT("Used")))
		{
			StartPoint = Tmp;
			Tmp->Tags.Add(TEXT("Used"));
			break;
		}
	}
	if (!StartPoint)
	{
		StartPoint = FoundStarts[0];
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetPawn();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Loc = StartPoint->GetActorLocation();
	FRotator Rot = StartPoint->GetActorRotation();
	ACharacter* SpawnedCharacter = World->SpawnActor<ACharacter>(InClass, Loc, Rot, SpawnParams);

	OnPossess(SpawnedCharacter);
}

void ADemoPlayerController::LeftMouseAction()
{
	if (auto DemoChr = Cast<ADemoCharacter>(GetPawn()))
	{
		DemoChr->OnHitBlockActionDelegate.Broadcast();
	}
}

void ADemoPlayerController::RightMouseAction()
{
	if (auto DemoChr = Cast<ADemoCharacter>(GetPawn()))
	{
		DemoChr->OnPlaceBlockActionDelegate.Broadcast();
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

// Developer: Jichao Luo


#include "DemoCharacter.h"
#include "MinecraftDemo/Controller/DemoPlayerController.h"
#include "MotionMatchingAls/3C/Camera/MMAlsCameraComponent.h"
#include "MinecraftPlugin/Game/MCVoxelGameState.h"
#include "MinecraftPlugin/Actor/Chunk/MCChunkManager.h"
#include "MinecraftPlugin/Actor/Chunk/MCChunkBase.h"
#include "MinecraftPlugin/Inventory/Item/MCItem.h"
#include "MinecraftPlugin/Inventory/Item/MCItemDatabase.h"
#include "MinecraftPlugin/Inventory/MCInventory.h"
#include "MinecraftPlugin/Inventory/MCPlayerInventory.h"
#include "MinecraftPlugin/UI/HUD/MCVoxelHUD.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryVisualizerWidget.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCPlayerHotbarWidget.h"
#include "MinecraftPlugin/Library/MCVoxelUtilsLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


ADemoCharacter::ADemoCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(GetMesh());

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head(Expression)"));
	Head->SetupAttachment(Body, FName("head"));
	Head->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

	Camera->SetupAttachment(Body);
	Camera->SetRelativeLocation(FVector(0.f, 25.f, 170.f));
	Camera->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	LeftHandScene = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandScene"));
	LeftHandScene->SetupAttachment(Camera);
	LeftHandScene->SetRelativeLocation(FVector(45.f, -45.f, -20.f));
	LeftHandScene->SetRelativeRotation(FRotator(30.f, 185.f, 90.f));
	LeftHandScene->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));

	LeftHandObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHandObject"));
	LeftHandObject->SetupAttachment(LeftHandScene);

	RightHandScene = CreateDefaultSubobject<USceneComponent>(TEXT("RightHandScene"));
	RightHandScene->SetupAttachment(Camera);
	RightHandScene->SetRelativeLocation(FVector(45.f, 45.f, -20.f));
	RightHandScene->SetRelativeRotation(FRotator(30.f, 175.f, 90.f));
	RightHandScene->SetRelativeScale3D(FVector(0.45f, 0.45f, 0.45f));

	RightHandObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandObject"));
	RightHandObject->SetupAttachment(RightHandScene);
}

void ADemoCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Camera->GetViewMode() == EMMAlsViewMode::FirstPerson)
	{
		Camera->SetFPOverrideByCameraLoc(true);
	}

	GetMesh()->SetCastShadow(false);
	Body->SetCastHiddenShadow(true);
	Body->SetAffectIndirectLightingWhileHidden(true);
	Body->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::None);
	Head->SetCastHiddenShadow(true);
	Head->SetAffectIndirectLightingWhileHidden(true);
	Head->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::None);
	LeftHandObject->SetCastShadow(false);
	RightHandObject->SetCastShadow(false);

	UpdateMeshesRenderingMode();
}

void ADemoCharacter::UpdateMeshesRenderingMode()
{
	if (Camera->GetViewMode() == EMMAlsViewMode::FirstPerson)
	{
		Body->SetOwnerNoSee(true);
		Head->SetOwnerNoSee(true);
	}
	else
	{
		Body->SetOwnerNoSee(false);
		Head->SetOwnerNoSee(false);
	}
}

void ADemoCharacter::SetHandsObjectVisibility(const bool bVisible)
{
	LeftHandObject->SetVisibility(bVisible, true);
	RightHandObject->SetVisibility(bVisible, true);
}

void ADemoCharacter::SetBodyOwnerNoSee(const bool bIsNoSee)
{
	Body->SetOwnerNoSee(bIsNoSee);
	Head->SetOwnerNoSee(bIsNoSee);
}

void ADemoCharacter::BeginPlay()
{
	Super::BeginPlay();

	Camera->IsOwnerNoSeeChangedDelegate.AddDynamic(this, &ThisClass::OnIsNoSeeChanged);

	if (IsLocallyControlled())
	{
		LeftHandObject->SetVisibility(bForceHideHandsObject ? false : true, true);
		RightHandObject->SetVisibility(bForceHideHandsObject ? false : true, true);
	}
	else
	{
		LeftHandObject->SetVisibility(false, true);
		RightHandObject->SetVisibility(false, true);
	}
}

void ADemoCharacter::OnIsNoSeeChanged(bool bIsOwnerNoSee)
{
	Body->SetOwnerNoSee(bIsOwnerNoSee);
	Head->SetOwnerNoSee(bIsOwnerNoSee);
}

void ADemoCharacter::OnHitActionComplete()
{
	auto PC = Cast<APlayerController>(Controller);
	if (!IsValid(PC)) return;

	int32 ViewportX, ViewportY;
	PC->GetViewportSize(ViewportX, ViewportY);
	FVector2D ScreenCenter(ViewportX / 2.f, ViewportY / 2.f);

	FVector WorldOrigin, WorldDirection;
	if (UGameplayStatics::DeprojectScreenToWorld(PC, ScreenCenter, WorldOrigin, WorldDirection))
	{
		FVector Start = WorldOrigin;
		FVector End = Start + (WorldDirection * 2000.f);
		HitBlock(Start, End);
	}
}

void ADemoCharacter::HitBlock(const FVector& TraceStart, const FVector& TraceEnd)
{
	if (HasAuthority())
	{
		Multicast_HitBlock(TraceStart, TraceEnd);
	}
	else
	{
		Server_HitBlock(TraceStart, TraceEnd);
	}
}

void ADemoCharacter::Server_HitBlock_Implementation(const FVector TraceStart, const FVector TraceEnd)
{
	Multicast_HitBlock(TraceStart, TraceEnd);
}

void ADemoCharacter::Multicast_HitBlock_Implementation(const FVector TraceStart, const FVector TraceEnd)
{
	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		TraceStart,
		TraceEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		{},
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	if (!bHit) return;
	if (!HitResult.GetActor()->ActorHasTag(AMCChunkManager::DefaultChunkTag)) return;
	if (FVector::Dist(HitResult.ImpactPoint, GetActorLocation()) > HitDistance) return;
	AMCChunkBase* HitChunk = Cast<AMCChunkBase>(HitResult.GetActor());
	if (HitChunk)
	{
		HitChunk->ModifyVoxel(
			UMCVoxelUtilsLibrary::WorldToLocalBlockPosition(HitResult.Location - HitResult.Normal, HitChunk->GetChunkSize()),
			EMCBlock::Air
		);
	}
}

void ADemoCharacter::OnPlaceActionComplete()
{
	auto PC = Cast<ADemoPlayerController>(Controller);
	if (!IsValid(PC)) return;

	int32 ViewportX, ViewportY;
	PC->GetViewportSize(ViewportX, ViewportY);
	FVector2D ScreenCenter(ViewportX / 2.f, ViewportY / 2.f);

	FVector WorldOrigin, WorldDirection;
	if (UGameplayStatics::DeprojectScreenToWorld(PC, ScreenCenter, WorldOrigin, WorldDirection))
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
			if (FVector::Dist(HitResult.ImpactPoint, GetActorLocation()) < 500.f)
			{
				auto GS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState());
				if (!IsValid(GS)) return;

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
						Cast<AMCVoxelHUD>(PC->GetHUD())->GetInventoryVisualizer()->ToggleBothInventories(
							GS->GetPlayerInventory(),
							WorldInventory,
							bIsInventoryDisplayed
						);
						PC->UpdateInteractInputMode(bIsInventoryDisplayed);
					}
					break;
				}
				default:
				{
					PlaceBlock(Start, End);
				}
				}
			}
		}
	}
}

void ADemoCharacter::PlaceBlock(const FVector& TraceStart, const FVector& TraceEnd)
{
	if (HasAuthority())
	{
		Multicast_PlaceBlock(TraceStart, TraceEnd);
	}
	else
	{
		Server_PlaceBlock(TraceStart, TraceEnd);
	}
}

void ADemoCharacter::Server_PlaceBlock_Implementation(const FVector TraceStart, const FVector TraceEnd)
{
	Multicast_PlaceBlock(TraceStart, TraceEnd);
}

void ADemoCharacter::Multicast_PlaceBlock_Implementation(const FVector TraceStart, const FVector TraceEnd)
{
	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		TraceStart,
		TraceEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		{},
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	if (!bHit) return;
	if (!HitResult.GetActor()->ActorHasTag(AMCChunkManager::DefaultChunkTag)) return;
	if (FVector::Dist(HitResult.ImpactPoint, GetActorLocation()) > 500.f) return;

	auto GS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState());
	auto PC = Cast<ADemoPlayerController>(Controller);
	AMCChunkBase* HitChunk = Cast<AMCChunkBase>(HitResult.GetActor());

	UMCItem* RemovedItem;
	auto PlayerInventory = GS->GetPlayerInventory();
	if (PlayerInventory->RemoveNumberFrom(PlayerInventory->GetCurrentlySelectedHotbarSlotCoords(), 1, RemovedItem))
	{
		auto Block = GS->GetItemInfoDatabase()->GetItemInfo(RemovedItem->GetAssociatedItemID()).BlockType;
		PlaceBlock(HitChunk, HitResult.Location - HitResult.Normal, HitResult.Normal, Block);
		if (Controller && Controller->IsLocalController())
		{
			Cast<AMCVoxelHUD>(PC->GetHUD())->GetPlayerHotbar()->UpdateItems();
		}
	}
}

void ADemoCharacter::PlaceBlock(AMCChunkBase* Chunk, const FVector& WorldPos, const FVector& HitNormal, EMCBlock Block)
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

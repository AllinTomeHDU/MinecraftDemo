// Developer: Jichao Luo


#include "DemoCharacter.h"
#include "MinecraftDemo/Controller/DemoPlayerController.h"
#include "MotionMatchingAls/3C/Camera/MMAlsCameraComponent.h"
#include "MinecraftPlugin/Actor/Chunk/MCChunkManager.h"
#include "MinecraftPlugin/Actor/Chunk/MCChunkBase.h"
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

		LeftHandObject->SetVisibility(true);
		RightHandObject->SetVisibility(true);
	}
	else
	{
		Body->SetOwnerNoSee(false);
		Head->SetOwnerNoSee(false);

		LeftHandObject->SetVisibility(false);
		RightHandObject->SetVisibility(false);
	}
}

void ADemoCharacter::BeginPlay()
{
	Super::BeginPlay();

	Camera->IsOwnerNoSeeChangedDelegate.AddDynamic(this, &ThisClass::OnIsNoSeeChanged);

	LeftHandObject->SetVisibility(IsLocallyControlled() ? true : false, true);
	RightHandObject->SetVisibility(IsLocallyControlled() ? true : false, true);
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

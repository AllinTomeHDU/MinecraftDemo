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

	Body->SetVisibility(false);
	Body->SetCastHiddenShadow(true);
	Body->SetAffectIndirectLightingWhileHidden(true);
	Body->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::None);

	Head->SetVisibility(false);
	Head->SetCastHiddenShadow(true);
	Head->SetAffectIndirectLightingWhileHidden(true);
	Head->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::None);

	LeftHandObject->SetCastShadow(false);
	RightHandObject->SetCastShadow(false);
}

void ADemoCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ADemoCharacter::OnHitActionComplete()
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * Reach);

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
		HitBlock(HitResult);
	}
}

void ADemoCharacter::HitBlock(const FHitResult& HitResult)
{
	AMCChunkBase* HitChunk = Cast<AMCChunkBase>(HitResult.GetActor());
	check(HitChunk);

	HitChunk->ModifyVoxel(
		UMCVoxelUtilsLibrary::WorldToLocalBlockPosition(HitResult.Location - HitResult.Normal, HitChunk->GetChunkSize()),
		EMCBlock::Air
	);
}

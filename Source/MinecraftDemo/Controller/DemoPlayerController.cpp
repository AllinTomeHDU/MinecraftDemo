// Developer: Jichao Luo


#include "DemoPlayerController.h"
#include "MotionMatchingAls/3C/Character/MMAlsCharacter.h"
#include "MotionMatchingAls/3C/Camera/MMAlsCameraComponent.h"
#include "MinecraftPlugin/Actor/Chunk/MCChunkManager.h"
#include "MinecraftPlugin/Actor/Chunk/MCChunkBase.h"
#include "MinecraftPlugin/Library/MCVoxelUtilsLibrary.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"


ADemoPlayerController::ADemoPlayerController()
{

}

void ADemoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComp)
	{
		EnhancedInputComp->BindAction(IA_LeftMouse, ETriggerEvent::Started, this, &ThisClass::LeftMouseAction);
		EnhancedInputComp->BindAction(IA_RightMouse, ETriggerEvent::Started, this, &ThisClass::RightMouseAction);
	}

	auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(IMC_MCDemo, MCDemoInputsPriority);
	}
}

void ADemoPlayerController::LeftMouseAction()
{
	DeleteChunk();
}

void ADemoPlayerController::RightMouseAction()
{
	CreateChunk();
}

void ADemoPlayerController::DeleteChunk()
{
	if (!Chr || !Chr->GetCameraComponent()) return;

	auto Camera = Chr->GetCameraComponent();
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * Reach);

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		{ Chr },
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	if (!bHit || !HitResult.GetActor()->ActorHasTag(AMCChunkManager::DefaultChunkTag)) return;

	AMCChunkBase* HitChunk = Cast<AMCChunkBase>(HitResult.GetActor());
	check(HitChunk);

	HitChunk->ModifyVoxel(
		UMCVoxelUtilsLibrary::WorldToLocalBlockPosition(HitResult.Location - HitResult.Normal, HitChunk->GetChunkSize()),
		EMCBlock::Air
	);
}

void ADemoPlayerController::CreateChunk()
{
	

}

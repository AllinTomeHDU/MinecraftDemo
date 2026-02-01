// Developer: Jichao Luo


#include "DemoPlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, TEXT("Delete Chunk Action Triggered"));

}

void ADemoPlayerController::CreateChunk()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Create Chunk Action Triggered"));

}

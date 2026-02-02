// Developer: Jichao Luo


#include "DemoPlayerController.h"
#include "MotionMatchingAls/3C/Character/MMAlsCharacter.h"
#include "MotionMatchingAls/3C/Camera/MMAlsCameraComponent.h"
#include "MinecraftPlugin/Game/MCVoxelGameState.h"
#include "MinecraftPlugin/UI/HUD/MCVoxelHUD.h"
#include "MinecraftPlugin/UI/Widget/Inventory/MCInventoryVisualizerWidget.h"
#include "MinecraftPlugin/Inventory/MCPlayerInventory.h"
#include "MinecraftPlugin/Inventory/MCInventoryInterface.h"
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
		EnhancedInputComp->BindAction(IA_LeftMouse, ETriggerEvent::Triggered, this, &ThisClass::LeftMouseAction);
		EnhancedInputComp->BindAction(IA_RightMouse, ETriggerEvent::Started, this, &ThisClass::RightMouseAction);
		EnhancedInputComp->BindAction(IA_Inventory, ETriggerEvent::Started, this, &ThisClass::InventoryActoin);
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
	CreateChunk();
}

void ADemoPlayerController::CreateChunk()
{
	

}

void ADemoPlayerController::InventoryActoin()
{
	auto VoxelHUD = Cast<AMCVoxelHUD>(GetHUD());
	auto VoxelGS = Cast<AMCVoxelGameState>(GetWorld()->GetGameState());
	if (!IsValid(VoxelHUD) || !IsValid(VoxelGS)) return;

	bool bIsDisplayed;
	VoxelHUD->GetInventoryVisualizer()->ToggleInventoryState(VoxelGS->GetPlayerInventory(), bIsDisplayed);

	UpdateInteractInputMode(bIsDisplayed);
}

void ADemoPlayerController::UpdateInteractInputMode(const bool bIsDisplayed)
{
	//if (bIsDisplayed)
	//{
	//	SetInputMode(FInputModeUIOnly());
	//	SetShowMouseCursor(true);
	//}
	//else
	//{
	//	SetInputMode(FInputModeGameOnly());
	//	SetShowMouseCursor(false);
	//}
}

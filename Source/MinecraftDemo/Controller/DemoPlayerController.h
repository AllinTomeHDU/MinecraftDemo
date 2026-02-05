// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "MotionMatchingAls/3C/Controller/MMAlsPlayerController.h"
#include "MinecraftPlugin/Library/Definition/MCPluginTypes.h"
#include "DemoPlayerController.generated.h"

class AMCChunkBase;

/**
 * 
 */
UCLASS()
class MINECRAFTDEMO_API ADemoPlayerController : public AMMAlsPlayerController
{
	GENERATED_BODY()

public:
	ADemoPlayerController();

	void UpdateInteractInputMode(const bool bIsDisplayed);

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_SpawnHeroCharacter(UClass* InClass);

	void LeftMouseAction();
	void RightMouseAction();
	void InventoryActoin();
	void HotbarCursorActrion(const FInputActionValue& Value);
	void ToggleViewModeAction();
	void ToggleLocomotionMode();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Demo")
	TSubclassOf<ACharacter> DefaultCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs|Demo")
	UInputMappingContext* IMC_MCDemo;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs|Demo")
	int32 MCDemoInputsPriority = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs|Demo")
	UInputAction* IA_LeftMouse;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs|Demo")
	UInputAction* IA_RightMouse;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs|Demo")
	UInputAction* IA_Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs|Demo")
	UInputAction* IA_HotbarCursor;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs|Demo")
	UInputAction* IA_CameraMode;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs|Demo")
	UInputAction* IA_LocomotionMode;

	UPROPERTY(EditAnywhere)
	float Reach = 4.f * 100.f;
};

// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "MotionMatchingAls/3C/Controller/MMAlsPlayerController.h"
#include "MinecraftPlugin/Library/Definition/MCPluginTypes.h"
#include "DemoPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftMouseTriggerDelegate);

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
	
	UPROPERTY(BlueprintAssignable)
	FOnLeftMouseTriggerDelegate OnLeftMouseTrigger;

protected:
	virtual void SetupInputComponent() override;

	void LeftMouseAction();

	void RightMouseAction();
	void PlaceBlock(AMCChunkBase* Chunk, const FVector& WorldPos, const FVector& HitNormal, EMCBlock Block);

	void InventoryActoin();
	void UpdateInteractInputMode(const bool bIsDisplayed);

	void HotbarCursorActrion(const FInputActionValue& Value);

	void ToggleViewModeAction();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	UInputMappingContext* IMC_MCDemo;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	int32 MCDemoInputsPriority = 2;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	UInputAction* IA_LeftMouse;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	UInputAction* IA_RightMouse;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	UInputAction* IA_Inventory;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	UInputAction* IA_HotbarCursor;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	UInputAction* IA_CameraMode;

	UPROPERTY(EditAnywhere)
	float Reach = 4.f * 100.f;
};

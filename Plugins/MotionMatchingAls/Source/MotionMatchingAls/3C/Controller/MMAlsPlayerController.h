// Developer: Luoo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "MMAlsPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class MOTIONMATCHINGALS_API AMMAlsPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMMAlsPlayerController();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	void SetupLocomotionInputs();
	void SetupDebugInputs();

	void MoveAction(const FInputActionValue& Value);
	void LookAction(const FInputActionValue& Value);

	void WalkRunSwitchAction(const FInputActionValue& Value);
	void SprintingAction(const FInputActionValue& Value);
	void SprintingEndAction(const FInputActionValue& Value);

	void CrouchingAction(const FInputActionValue& Value);
	void CrouchingEndAction(const FInputActionValue& Value);

	void AimingAction(const FInputActionValue& Value);
	void AimingEndAction(const FInputActionValue& Value);

	void JumpAction(const FInputActionValue& Value);

	void RotationModeAction(const FInputActionValue& Value);
	void ViewModeAction(const FInputActionValue& Value);
	void ShoulderModeAction(const FInputActionValue& Value);
	void OverlayBaseAction(const FInputActionValue& Value);
	void OverlayPoseAction(const FInputActionValue& Value);
	void RagdollingAction(const FInputActionValue& Value);

	UFUNCTION()
	void OnRep_Chr();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Chr)
	class AMMAlsCharacter* Chr;

	UPROPERTY()
	class UMMAlsMovementComponent* MoveComp;

private:
	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Settings")
	FVector2D ViewPitchRange = FVector2D(-85.f, 90.f);

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Locomotion")
	UInputMappingContext* IMC_Locomotion;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Locomotion")
	int32 LocomotionInputsPriority = 1;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Locomotion")
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Locomotion")
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Locomotion")
	UInputAction* IA_Gait;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Locomotion")
	UInputAction* IA_Stance;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Locomotion")
	UInputAction* IA_Aiming;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Locomotion")
	UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Debug")
	bool bEnableDebugInputs;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Debug")
	class UInputMappingContext* IMC_Debug;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Debug")
	int32 DebugInputsPriority = 0;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Debug")
	UInputAction* IA_RotationMode;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Debug")
	UInputAction* IA_ViewMode;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Debug")
	UInputAction* IA_ShoulderMode;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Debug")
	UInputAction* IA_OverlayBase;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Debug")
	UInputAction* IA_OverlayPose;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Debug")
	UInputAction* IA_Ragdoll;
};


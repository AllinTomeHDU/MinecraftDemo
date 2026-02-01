// Developer: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "MotionMatchingAls/3C/Controller/MMAlsPlayerController.h"
#include "DemoPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFTDEMO_API ADemoPlayerController : public AMMAlsPlayerController
{
	GENERATED_BODY()

public:
	ADemoPlayerController();
	
protected:
	virtual void SetupInputComponent() override;

	void LeftMouseAction();
	void RightMouseAction();

	void DeleteChunk();
	void CreateChunk();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	UInputMappingContext* IMC_MCDemo;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	int32 MCDemoInputsPriority = 2;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	UInputAction* IA_LeftMouse;

	UPROPERTY(EditDefaultsOnly, Category = "MMAls|Inputs|Demo")
	UInputAction* IA_RightMouse;

	UPROPERTY(EditAnywhere)
	float Reach = 5.f * 100.f;
};

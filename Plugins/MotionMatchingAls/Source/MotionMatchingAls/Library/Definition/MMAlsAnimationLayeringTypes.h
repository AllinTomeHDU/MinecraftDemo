#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MMAlsAnimationLayeringTypes.generated.h"


USTRUCT(BlueprintType)
struct MOTIONMATCHINGALS_API FMMAlsLayeringCurves
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm", Meta = (ClampMin = 0, ClampMax = 1))
	float ArmLeft{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm", Meta = (ClampMin = 0, ClampMax = 1))
	float ArmRight{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm", Meta = (ClampMin = 0, ClampMax = 1))
	float ArmLeft_LS{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm", Meta = (ClampMin = 0, ClampMax = 1))
	float ArmRight_LS{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm", Meta = (ClampMin = 0, ClampMax = 1))
	float ArmLeft_MS{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arm", Meta = (ClampMin = 0, ClampMax = 1))
	float ArmRight_MS{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hand", Meta = (ClampMin = 0, ClampMax = 1))
	float HandLeft{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hand", Meta = (ClampMin = 0, ClampMax = 1))
	float HandRight{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HandIK", Meta = (ClampMin = 0, ClampMax = 1))
	float EnableHandIK_L{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HandIK", Meta = (ClampMin = 0, ClampMax = 1))
	float EnableHandIK_R{ 0 };
};

UCLASS(Meta = (BlueprintThreadSafe))
class MOTIONMATCHINGALS_API UMMAlsLayeringContants : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Meta = (ReturnDisplayName = "Curve Name"))
	static const FName& LayerArmLeftCurveName() { return LayerArmLeftCurve; }

	UFUNCTION(BlueprintPure, Meta = (ReturnDisplayName = "Curve Name"))
	static const FName& LayerArmRightCurveName() { return LayerArmRightCurve; }

	UFUNCTION(BlueprintPure, Meta = (ReturnDisplayName = "Curve Name"))
	static const FName& LayerArmLeftCurveName_LS() { return LayerArmLeftCurve_LS; }

	UFUNCTION(BlueprintPure, Meta = (ReturnDisplayName = "Curve Name"))
	static const FName& LayerArmRightCurveName_LS() { return LayerArmRightCurve_LS; }

	UFUNCTION(BlueprintPure, Meta = (ReturnDisplayName = "Curve Name"))
	static const FName& LayerHandLeftCurveName() { return LayerHandLeftCurve; }

	UFUNCTION(BlueprintPure, Meta = (ReturnDisplayName = "Curve Name"))
	static const FName& LayerHandRightCurveName() { return LayerHandRightCurve; }

	UFUNCTION(BlueprintPure, Meta = (ReturnDisplayName = "Curve Name"))
	static const FName& EnableHandLeftIkCurveName() { return EnableHandLeftIkCurve; }

	UFUNCTION(BlueprintPure, Meta = (ReturnDisplayName = "Curve Name"))
	static const FName& EnableHandRightIkCurveName() { return EnableHandRightIkCurve; }

protected:
	inline static const FName LayerArmLeftCurve{ TEXTVIEW("Layering_Arm_L") };
	inline static const FName LayerArmRightCurve{ TEXTVIEW("Layering_Arm_R") };

	inline static const FName LayerArmLeftCurve_LS{ TEXTVIEW("Layering_Arm_L_LS") };
	inline static const FName LayerArmRightCurve_LS{ TEXTVIEW("Layering_Arm_R_LS") };

	inline static const FName LayerHandLeftCurve{ TEXTVIEW("Layering_Hand_L") };
	inline static const FName LayerHandRightCurve{ TEXTVIEW("Layering_Hand_R") };

	inline static const FName EnableHandLeftIkCurve{ TEXTVIEW("Enable_HandIK_L") };
	inline static const FName EnableHandRightIkCurve{ TEXTVIEW("Enable_HandIK_R") };

};
// Copyright: Jichao Luo

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Proxy/MMAlsAnimInstanceProxyBase.h"
#include "MotionMatchingAls/Library/Definition/MMAlsAnimationTypes.h"
#include "MMAlsAnimInstanceBase.generated.h"

/**
 * 仅仅只对角色的基本运动数据与状态做更新，不进行具体的运动技术选型
 */
UCLASS()
class MOTIONMATCHINGALS_API UMMAlsAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
	
	friend struct FMMAlsAnimInstanceProxyBase;

	UPROPERTY(Transient)
	FMMAlsAnimInstanceProxyBase ProxyBase;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override { return &ProxyBase; }
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void UpdateTraversalStates();
	virtual void UpdateOffsetRootBone();
	virtual void UpdateFootPlacement();
	virtual void UpdateHandIK();
	
protected:
	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AMMAlsCharacter* Chr;

	UPROPERTY(Transient, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMMAlsMovementComponent* MoveComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MMAls|Settings", meta = (AllowPrivateAccess = "true"))
	EMMAlsAnimationLOD AnimationLOD = EMMAlsAnimationLOD::Dense;

	/*
	* Essential
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|Essential", meta = (AllowPrivateAccess = "true"))
	FMMAlsEssentialStates EssentialStates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|Essential", meta = (AllowPrivateAccess = "true"))
	FMMAlsEssentialValues EssentialValues;
	
	UPROPERTY(EditAnywhere, Category = "MMAls|Essential")
	float StartStateTimer{ 0.3f };

	UPROPERTY(EditAnywhere, Category = "MMAls|Essential")
	float StopStateTimer{ 0.2f };

	UPROPERTY(EditAnywhere, Category = "MMAls|Essential")
	float PivotStateTimer{ 0.2f };

	/*
	* Ragdoll
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|Ragdoll", meta = (AllowPrivateAccess = "true"))
	FMMAlsRagdollValues RagdollValues;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|Ragdoll", meta = (AllowPrivateAccess = "true"))
	float FlailRate;

	/*
	* Traversal
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|Traversal", meta = (AllowPrivateAccess = "true"))
	FMMAlsTraversalStates TraversalStates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|Traversal", meta = (AllowPrivateAccess = "true"))
	FTransform TraversalAttachTransform;

	/*
	* Additive
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|Lean", meta = (AllowPrivateAccess = "true"))
	float LeanValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|AimOffset", meta = (AllowPrivateAccess = "true"))
	FMMAlsAimOffset AimOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|OffsetRootBone", meta = (AllowPrivateAccess = "true"))
	FMMAlsOffsetRootBone OffsetRootBone;

	/*
	* Random Idle
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|PickRandomAnimCurve", meta = (AllowPrivateAccess = "true"))
	float RandomIdleCurve;

	/*
	* IK
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MMAls|FootPlacement", meta = (AllowPrivateAccess = "true"))
	FMMAlsFootPlacement FootPlacement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|Layering", meta = (AllowPrivateAccess = "true"))
	FMMAlsHandIK HandIK;

	/*
	* Layering
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|Layering", meta = (AllowPrivateAccess = "true"))
	FMMAlsLayeringCurves LayeringCurves;

public:
	FORCEINLINE EMMAlsAnimationLOD GetAnimationLOD() const { return AnimationLOD; }
	FORCEINLINE FMMAlsRagdollValues GetRagdollingValues() const { return RagdollValues; }
	FORCEINLINE FMMAlsAimOffset GetAimOffset() const { return AimOffset; }
	FORCEINLINE FTransform GetTraversalAttachTransform() const { return TraversalAttachTransform; }
};

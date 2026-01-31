#pragma once

#include "Animation/AnimInstanceProxy.h"
#include "MotionMatchingAls/Library/Definition/MMAlsAnimationTypes.h"
#include "MMAlsAnimInstanceProxyBase.generated.h"

/**
 *
 */
USTRUCT()
struct MOTIONMATCHINGALS_API FMMAlsAnimInstanceProxyBase : public FAnimInstanceProxy
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	class AMMAlsCharacter* Chr;

	UPROPERTY(Transient)
	class UMMAlsMovementComponent* MoveComp;

	UPROPERTY(Transient)
	class UMMAlsAnimInstanceBase* AnimInstBase;

protected:
	virtual void InitializeObjects(UAnimInstance* InAnimInstance) override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaTime) override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(UAnimInstance* InAnimInstance) const override;

protected:
	virtual void UpdateEssentialStates();
	virtual void UpdateEssentialValues();
	virtual void UpdateRagdollValues();
	virtual void UpdateLayeringCurves();

	virtual void UpdateLeanValue() const;
	virtual void UpdateAimOffset() const;
	virtual void UpdateAimCamera() const;


protected:
	UPROPERTY(Transient)
	FMMAlsEssentialStates CachedEssentialStates;

	UPROPERTY(Transient)
	FMMAlsEssentialValues CachedEssentialValues;

	UPROPERTY(Transient)
	FMMAlsEssentialStates EssentialStates;

	UPROPERTY(Transient)
	FMMAlsEssentialValues EssentialValues;

	UPROPERTY(Transient)
	FMMAlsLayeringCurves LayeringCurves;

private:
	UPROPERTY(Transient)
	float StartStateTimer{ 0.f };

	UPROPERTY(Transient)
	float StopStateTimer{ 0.f };

	UPROPERTY(Transient)
	float PivotStateTimer{ 0.f };
};
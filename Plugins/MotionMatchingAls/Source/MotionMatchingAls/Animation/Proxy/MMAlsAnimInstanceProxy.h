// Developer: Luoo

#pragma once

#include "MMAlsAnimInstanceProxyBase.h"
#include "MMAlsAnimInstanceProxy.generated.h"

/**
 * 
 */
USTRUCT()
struct MOTIONMATCHINGALS_API FMMAlsAnimInstanceProxy : public FMMAlsAnimInstanceProxyBase
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	class UMMAlsAnimInstance* AnimInst;

	struct FAnimNode_MotionMatching* MotionMatchingNode;

protected:
	virtual void InitializeObjects(UAnimInstance* InAnimInstance) override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaTime) override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(UAnimInstance* InAnimInstance) const override;

	virtual void UpdateEssentialValues() override;

private:
	void UpdateTrajectoryData();
	void UpdateTrajectory(float DeltaTime);

	void UpdateMotionMatchingDatabases();
	void UpdateMotionMatching();
	void UpdateMotionMatchingBlendTime() const;
	void UpdateMotionMatchingState() const;
	void UpdatePoseSearchInterruptMode();

private:
	UPROPERTY(Transient)
	FTransformTrajectory Trajectory;

	UPROPERTY(Transient)
	FPoseSearchTrajectoryData TrajectoryData;

	FPoseSearchTrajectoryData::FSampling TrajSampling;
	FPoseSearchTrajectoryData::FDerived TrajDerived;
	FPoseSearchTrajectoryData::FState TrajState;

	UPROPERTY(Transient)
	TArray<class UPoseSearchDatabase*> Databases;

	EPoseSearchInterruptMode PoseSearchInterruptMode;
};
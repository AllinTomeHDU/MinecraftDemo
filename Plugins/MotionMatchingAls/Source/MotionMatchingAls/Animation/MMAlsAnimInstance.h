// Developer: Luoo

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MMAlsAnimInstanceBase.h"
#include "Proxy/MMAlsAnimInstanceProxy.h"
#include "MMAlsAnimInstance.generated.h"

class UPoseSearchDatabase;

/**
 * 
 */
UCLASS()
class MOTIONMATCHINGALS_API UMMAlsAnimInstance : public UMMAlsAnimInstanceBase
{
	GENERATED_BODY()

	friend struct FMMAlsAnimInstanceProxy;

	UPROPERTY(Transient)
	FMMAlsAnimInstanceProxy Proxy;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override { return &Proxy; }
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

public:
	void SetTraversalAttachTransform(const FTransform& NewTransform);

	float GetTrajectoryTurnAngle();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void UpdateFootPlacement() override;

	void UpdatePoseSearchTrajectoryCollision();
	void UpdatePoseSearchTrajectoryVelocity();

	void UpdateMotionMatchingDatabases(TArray<class UPoseSearchDatabase*>& OutDatabases);

private:
	struct FAnimNode_MotionMatching* MotionMatchingNode;

	/*
	* Trajectory
	*/
	UPROPERTY(EditAnywhere, Category = "MMAls|Trajectory|Settings")
	FMMAlsTrajectorySettings TrajectorySettings;

	UPROPERTY(EditAnywhere, Category = "MMAls|Trajectory|Settings")
	FMMAlsTrajectoryCollisionSettings TrajectoryCollisionSettings;

	UPROPERTY(EditAnywhere, Category = "MMAls|Trajectory|Settings")
	FMMAlsTrajectoryVelocitySettings TrajectoryVelocitySettings;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|Trajectory", meta = (AllowPrivateAccess = "true"))
	FTransformTrajectory Trajectory;

	UPROPERTY(VisibleAnywhere, Category = "MMAls|Trajectory")
	float TrajectoryDesiredControllerYaw = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "MMAls|Trajectory")
	FPoseSearchTrajectory_WorldCollisionResults TrajectoryCollision;

	UPROPERTY(VisibleAnywhere, Category = "MMAls|Trajectory")
	FMMAlsTrajectoryVelocity TrajectoryVelocity;

	/*
	* Motion Matching
	*/
	UPROPERTY(EditAnywhere, Category = "MMAls|MotionMatching")
	class UChooserTable* PoseSearchChooserTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MMAls|MotionMatching", meta = (AllowPrivateAccess = "true"))
	float MotionMatchingBlendTime = 0.35f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MMAls|MotionMatching", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<const UPoseSearchDatabase> CurrentDatabase;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MMAls|MotionMatching", meta = (AllowPrivateAccess = "true"))
	TArray<FName> CurrentDatabaseTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MMAls|MotionMatching", meta = (AllowPrivateAccess = "true"))
	UAnimSequence* CurrentMatchedAnimation;

public:
	FORCEINLINE FAnimNode_MotionMatching* GetMotionMatchingNode() const { return MotionMatchingNode; }
	FORCEINLINE UChooserTable* GetPoseSearchChooserTable() const { return PoseSearchChooserTable; }
};


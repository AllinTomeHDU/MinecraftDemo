// Developer: Luoo


#include "MMAlsAnimInstanceProxy.h"
#include "../MMAlsAnimInstance.h"
#include "MotionMatchingAls/3C/Character/MMAlsCharacter.h"
#include "MotionMatchingAls/3C/Character/MMAlsMovementComponent.h"
#include "PoseSearch/AnimNode_MotionMatching.h"
#include "PoseSearch/PoseSearchDatabase.h"
#include "PoseSearch/MotionMatchingAnimNodeLibrary.h"
#include "Kismet/KismetMathLibrary.h"


void FMMAlsAnimInstanceProxy::InitializeObjects(UAnimInstance* InAnimInstance)
{
	FMMAlsAnimInstanceProxyBase::InitializeObjects(InAnimInstance);
	if (!Chr || !MoveComp || !AnimInstBase) return;

	AnimInst = Cast<UMMAlsAnimInstance>(InAnimInstance);
	MotionMatchingNode = IsValid(AnimInst) ? AnimInst->GetMotionMatchingNode() : nullptr;
}

void FMMAlsAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaTime)
{
	FMMAlsAnimInstanceProxyBase::PreUpdate(InAnimInstance, DeltaTime);
	if (!Chr || !MoveComp || !AnimInst) return;

	UpdateTrajectoryData();
	UpdateMotionMatchingDatabases();
}

void FMMAlsAnimInstanceProxy::Update(float DeltaTime)
{
	FMMAlsAnimInstanceProxyBase::Update(DeltaTime);
	if (!Chr || !MoveComp || !AnimInst) return;

	UpdateTrajectory(DeltaTime);
	UpdateMotionMatching();
	UpdateLayeringCurves();
}

void FMMAlsAnimInstanceProxy::PostUpdate(UAnimInstance* InAnimInstance) const
{
	FMMAlsAnimInstanceProxyBase::PostUpdate(InAnimInstance);

	if (!Chr || !MoveComp || !AnimInst) return;

	UpdateMotionMatchingBlendTime();
	UpdateMotionMatchingState();

	AnimInst->TrajectoryDesiredControllerYaw = TrajState.DesiredControllerYawLastUpdate;
	AnimInst->Trajectory = Trajectory;
	AnimInst->UpdatePoseSearchTrajectoryCollision();
	AnimInst->UpdatePoseSearchTrajectoryVelocity();
}

void FMMAlsAnimInstanceProxy::UpdateEssentialValues()
{
	FMMAlsAnimInstanceProxyBase::UpdateEssentialValues();

	EssentialValues.bIsMoving =
		!AnimInst->TrajectoryVelocity.FutureVelocity.Equals(FVector(0.f, 0.f, 0.f), 10.f) &&
		!EssentialValues.CurrentAcceleration.Equals(FVector(0.f, 0.f, 0.f), 0.f);

	EssentialValues.bIsStarting =
		EssentialValues.bIsMoving && !AnimInst->CurrentDatabaseTags.Contains(FName("Pivot")) &&
		AnimInst->TrajectoryVelocity.FutureVelocity.Size2D() >= EssentialValues.Speed + 100.f;

	float PivotThresholder =
		EssentialStates.RotationMode == EMMAlsRotationMode::VelocityDirection ? 45.f : 30.f;
	EssentialValues.bIsPivoting =
		EssentialValues.bIsMoving && FMath::Abs(AnimInst->GetTrajectoryTurnAngle()) >= PivotThresholder;
}

void FMMAlsAnimInstanceProxy::UpdateTrajectoryData()
{
	Trajectory = AnimInst->Trajectory;
	TrajectoryData = EssentialValues.bIsMoving ? AnimInst->TrajectorySettings.Move : AnimInst->TrajectorySettings.Idle;

	TrajSampling.NumHistorySamples = AnimInst->TrajectorySettings.TrajectorySampling.HistorySamplesNum;
	TrajSampling.SecondsPerHistorySample = AnimInst->TrajectorySettings.TrajectorySampling.HistorySampleSecondsPer;
	TrajSampling.NumPredictionSamples = AnimInst->TrajectorySettings.TrajectorySampling.PredictionSamplesNum;
	TrajSampling.SecondsPerPredictionSample = AnimInst->TrajectorySettings.TrajectorySampling.PredictionSampleSecondsPer;
	
	TrajState.DesiredControllerYawLastUpdate = AnimInst->TrajectoryDesiredControllerYaw;
	TrajectoryData.UpdateData(AnimInst->GetDeltaSeconds(), AnimInst, TrajDerived, TrajState);
}

void FMMAlsAnimInstanceProxy::UpdateTrajectory(float DeltaTime)
{
	UPoseSearchTrajectoryLibrary::InitTrajectorySamples(
		Trajectory,
		TrajDerived.Position,
		TrajDerived.Facing,
		TrajSampling,
		DeltaTime
	);
	UPoseSearchTrajectoryLibrary::UpdateHistory_TransformHistory(
		Trajectory,
		TrajDerived.Position,
		TrajDerived.Velocity,
		TrajSampling,
		DeltaTime
	);
	UPoseSearchTrajectoryLibrary::UpdatePrediction_SimulateCharacterMovement(
		Trajectory,
		TrajectoryData,
		TrajDerived,
		TrajSampling,
		DeltaTime
	);
}

void FMMAlsAnimInstanceProxy::UpdateMotionMatchingDatabases()
{
	AnimInst->UpdateMotionMatchingDatabases(Databases);
}

void FMMAlsAnimInstanceProxy::UpdateMotionMatching()
{
	if (MotionMatchingNode && !Databases.IsEmpty())
	{
		MotionMatchingNode->SetDatabasesToSearch(Databases, PoseSearchInterruptMode);
	}
	UpdatePoseSearchInterruptMode();
}

void FMMAlsAnimInstanceProxy::UpdateMotionMatchingBlendTime() const
{
	if (EssentialStates.MoveMode == EMMAlsMoveMode::OnGround)
	{
		AnimInst->MotionMatchingBlendTime =
			CachedEssentialStates.MoveMode == EMMAlsMoveMode::OnGround ? 0.5f : 0.2f;
	}
	else
	{
		AnimInst->MotionMatchingBlendTime = EssentialValues.Velocity.Z > 100.f ? 0.15f : 0.5f;
	}
}

void FMMAlsAnimInstanceProxy::UpdateMotionMatchingState() const
{
	if (MotionMatchingNode)
	{
		FMotionMatchingAnimNodeReference MotionMatchingRef;
		FAnimNodeReference GenericRef = FAnimNodeReference(AnimInst, *MotionMatchingNode);
		EAnimNodeReferenceConversionResult Result;
		MotionMatchingRef = UMotionMatchingAnimNodeLibrary::ConvertToMotionMatchingNode(GenericRef, Result);
		if (Result == EAnimNodeReferenceConversionResult::Succeeded)
		{
			bool bRes;
			FPoseSearchBlueprintResult SearchResult;
			UMotionMatchingAnimNodeLibrary::GetMotionMatchingSearchResult(MotionMatchingRef, SearchResult, bRes);
			if (bRes)
			{
				AnimInst->CurrentMatchedAnimation = Cast<UAnimSequence>(SearchResult.SelectedAnim);
				AnimInst->CurrentDatabase = SearchResult.SelectedDatabase;
				if (AnimInst->CurrentDatabase)
				{
					AnimInst->CurrentDatabaseTags = AnimInst->CurrentDatabase->Tags;
				}
			}
		}
	}
}

void FMMAlsAnimInstanceProxy::UpdatePoseSearchInterruptMode()
{
	if (CachedEssentialStates.MoveMode != EssentialStates.MoveMode)
	{
		PoseSearchInterruptMode = EPoseSearchInterruptMode::InterruptOnDatabaseChange;
	}
	else if (EssentialStates.MoveMode == EMMAlsMoveMode::OnGround)
	{
		if (CachedEssentialValues.bIsMoving != EssentialValues.bIsMoving ||
			CachedEssentialStates.Stance != EssentialStates.Stance)
		{
			PoseSearchInterruptMode = EPoseSearchInterruptMode::InterruptOnDatabaseChange;
		}
		else if (EssentialValues.bIsMoving && CachedEssentialStates.Gait != EssentialStates.Gait)
		{
			PoseSearchInterruptMode = EPoseSearchInterruptMode::InterruptOnDatabaseChange;
		}
		else
		{
			PoseSearchInterruptMode = EPoseSearchInterruptMode::DoNotInterrupt;
		}
	}
	else
	{
		PoseSearchInterruptMode = EPoseSearchInterruptMode::DoNotInterrupt;
	}
}


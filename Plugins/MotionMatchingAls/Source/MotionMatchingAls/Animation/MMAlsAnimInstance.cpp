// Developer: Luoo


#include "MMAlsAnimInstance.h"
#include "MotionMatchingAls/3C/Character/MMAlsCharacter.h"
#include "MotionMatchingAls/3C/Character/MMAlsMovementComponent.h"
#include "MotionMatchingAls/3C/Camera/MMAlsCameraComponent.h"
#include "MotionMatchingAls/3C/Camera/MMAlsCameraAnimInstance.h"
#include "PoseSearch/AnimNode_MotionMatching.h"
#include "PoseSearch/PoseSearchDatabase.h"
#include "Chooser/Internal/Chooser.h"
#include "Chooser/Public/ChooserFunctionLibrary.h"


void UMMAlsAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	for (TFieldIterator<FStructProperty> It(GetClass()); It; ++It)
	{
		FStructProperty* Prop = *It;
		if (Prop->Struct && Prop->Struct->IsChildOf(FAnimNode_MotionMatching::StaticStruct()))
		{
			void* NodePtr = Prop->ContainerPtrToValuePtr<void>(this);
			MotionMatchingNode = (FAnimNode_MotionMatching*)NodePtr;
			break;
		}
	}
}

void UMMAlsAnimInstance::UpdateFootPlacement()
{
	const bool bIsStop = CurrentDatabaseTags.Contains(FName("Stop"));
	FootPlacement.PlantSettings = bIsStop ? FootPlacement.PlantSettings_Stop
										  : FootPlacement.PlantSettings_Default;
	FootPlacement.InterpolationSettings = bIsStop ? FootPlacement.InterpolationSettings_Stop
												  : FootPlacement.InterpolationSettings_Default;
}

void UMMAlsAnimInstance::SetTraversalAttachTransform(const FTransform& NewTransform)
{
	TraversalAttachTransform = NewTransform;
}

float UMMAlsAnimInstance::GetTrajectoryTurnAngle()
{
	if (!IsValid(MoveComp))
	{
		MoveComp = Cast<UMMAlsMovementComponent>(Chr->GetCharacterMovement());
		if (!IsValid(MoveComp)) return 0.f;
	}

	return (TrajectoryVelocity.FutureVelocity.ToOrientationRotator() -
		MoveComp->Velocity.ToOrientationRotator()).GetNormalized().Yaw;
}

void UMMAlsAnimInstance::UpdatePoseSearchTrajectoryCollision()
{
	if (Trajectory.Samples.IsEmpty()) return;

	UPoseSearchTrajectoryLibrary::HandleTransformTrajectoryWorldCollisions(
		GetWorld(),
		this,
		Trajectory,
		TrajectoryCollisionSettings.bApplyGravity,
		TrajectoryCollisionSettings.FloorCollisionsOffset,
		Trajectory,
		TrajectoryCollision,
		UEngineTypes::ConvertToTraceType(TrajectoryCollisionSettings.TraceChannel),
		false,
		{},
		EDrawDebugTrace::None,
		true,
		TrajectoryCollisionSettings.MaxObstacleHeight
	);
}

void UMMAlsAnimInstance::UpdatePoseSearchTrajectoryVelocity()
{
	UPoseSearchTrajectoryLibrary::GetTransformTrajectoryVelocity(
		Trajectory,
		TrajectoryVelocitySettings.PastTime1,
		TrajectoryVelocitySettings.PastTime2,
		TrajectoryVelocity.PastVelocity
	);
	UPoseSearchTrajectoryLibrary::GetTransformTrajectoryVelocity(
		Trajectory,
		TrajectoryVelocitySettings.CurrentTime1,
		TrajectoryVelocitySettings.CurrentTime2,
		TrajectoryVelocity.CurrentVelocity
	);
	UPoseSearchTrajectoryLibrary::GetTransformTrajectoryVelocity(
		Trajectory,
		TrajectoryVelocitySettings.FutureTime1,
		TrajectoryVelocitySettings.FutureTime2,
		TrajectoryVelocity.FutureVelocity
	);
}

void UMMAlsAnimInstance::UpdateMotionMatchingDatabases(TArray<class UPoseSearchDatabase*>& OutDatabases)
{
	OutDatabases.Empty();
	FChooserEvaluationContext EvaluationContext;
	EvaluationContext.AddObjectParam(this);

	checkf(PoseSearchChooserTable, TEXT("PoseSearchChooserTable is not valid"));
	FInstancedStruct ChooserInstance = UChooserFunctionLibrary::MakeEvaluateChooser(PoseSearchChooserTable);
	TArray<UObject*> EvaluateResults = UChooserFunctionLibrary::EvaluateObjectChooserBaseMulti(
		EvaluationContext, ChooserInstance, UPoseSearchDatabase::StaticClass());
	for (UObject* Obj : EvaluateResults)
	{
		if (auto Database = Cast<UPoseSearchDatabase>(Obj))
		{
			OutDatabases.Add(Database);
		}
	}
}

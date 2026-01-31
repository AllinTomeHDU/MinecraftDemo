// Developer: Luoo


#include "MMAlsAnimInstanceProxyBase.h"
#include "../MMAlsAnimInstanceBase.h"
#include "MotionMatchingAls/3C/Character/MMAlsCharacter.h"
#include "MotionMatchingAls/3C/Character/MMAlsMovementComponent.h"
#include "MotionMatchingAls/3C/Camera/MMAlsCameraComponent.h"
#include "MotionMatchingAls/3C/Camera/MMAlsCameraAnimInstance.h"


void FMMAlsAnimInstanceProxyBase::InitializeObjects(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::InitializeObjects(InAnimInstance);

	Chr = Cast<AMMAlsCharacter>(InAnimInstance->GetOwningActor());
	if (!IsValid(Chr)) return;

	MoveComp = Cast<UMMAlsMovementComponent>(Chr->GetCharacterMovement());
	AnimInstBase = Cast<UMMAlsAnimInstanceBase>(InAnimInstance);
}

void FMMAlsAnimInstanceProxyBase::PreUpdate(UAnimInstance* InAnimInstance, float DeltaTime)
{
	FAnimInstanceProxy::PreUpdate(InAnimInstance, DeltaTime);

	if (!Chr)
	{
		Chr = Cast<AMMAlsCharacter>(InAnimInstance->GetOwningActor());
		if (!Chr) return;
	}
	if (!MoveComp)
	{
		MoveComp = Cast<UMMAlsMovementComponent>(Chr->GetCharacterMovement());
		if (!MoveComp) return;
	}
	if (!AnimInstBase)
	{
		AnimInstBase = Cast<UMMAlsAnimInstanceBase>(InAnimInstance);
		if (!AnimInstBase) return;
	}

	CachedEssentialStates = AnimInstBase->EssentialStates;
	CachedEssentialValues = AnimInstBase->EssentialValues;

	UpdateEssentialStates();
	UpdateEssentialValues();

	UpdateRagdollValues();
	UpdateLayeringCurves();
}

void FMMAlsAnimInstanceProxyBase::Update(float DeltaTime)
{
	FAnimInstanceProxy::Update(DeltaTime);

	if (!Chr || !MoveComp || !AnimInstBase) return;
}

void FMMAlsAnimInstanceProxyBase::PostUpdate(UAnimInstance* InAnimInstance) const
{
	FAnimInstanceProxy::PostUpdate(InAnimInstance);

	if (!Chr || !MoveComp || !AnimInstBase) return;

	UpdateLeanValue();
	UpdateAimOffset();
	UpdateAimCamera();

	AnimInstBase->EssentialValues = EssentialValues;
	AnimInstBase->EssentialStates = EssentialStates;
	AnimInstBase->RandomIdleCurve = MoveComp->GetRandomIdleCurve();
	AnimInstBase->LayeringCurves = LayeringCurves;

	AnimInstBase->UpdateTraversalStates();
	AnimInstBase->UpdateOffsetRootBone();
	AnimInstBase->UpdateFootPlacement();
	AnimInstBase->UpdateHandIK();
}

void FMMAlsAnimInstanceProxyBase::UpdateEssentialStates()
{
	EssentialStates.MoveMode = MoveComp->GetMoveMode();
	EssentialStates.Gait = MoveComp->GetGait();
	EssentialStates.Stance = MoveComp->GetStance();
	EssentialStates.RotationMode = MoveComp->GetRotationMode();
	EssentialStates.LandedInfo = MoveComp->GetLandedInfo();
	EssentialStates.bIsAiming = MoveComp->IsAiming();
	EssentialStates.OverlayBase = Chr->GetOverlayBase();
	EssentialStates.OverlayPose = Chr->GetOverlayPose();
}

void FMMAlsAnimInstanceProxyBase::UpdateEssentialValues()
{
	EssentialValues.Velocity = MoveComp->Velocity;
	EssentialValues.Speed = MoveComp->Velocity.Size2D();
	if (EssentialValues.Speed > 5.f)
	{
		EssentialValues.LastNonZeroVelocity = EssentialValues.Velocity;
	}

	EssentialValues.CurrentAcceleration = MoveComp->GetCurrentAcceleration();
	EssentialValues.MaxAccelerationAmount = MoveComp->GetMaxAcceleration();
	EssentialValues.MaxDecelerationAmount = MoveComp->GetMaxBrakingDeceleration();

	EssentialValues.bIsMoving = EssentialValues.Speed > 5.f && EssentialValues.CurrentAcceleration.Size() > 5.f;
	if (!CachedEssentialValues.bIsMoving && EssentialValues.bIsMoving)
	{
		EssentialValues.bIsStarting = true;
		StartStateTimer = AnimInstBase->StartStateTimer;
	}
	if (EssentialValues.bIsStarting)
	{
		if (StartStateTimer <= 0.f)
		{
			EssentialValues.bIsStarting = false;
		}
		else
		{
			StartStateTimer -= AnimInstBase->GetDeltaSeconds();
		}
	}

	if (EssentialValues.Speed < 150.f && EssentialValues.CurrentAcceleration.Size() > 10.f)
	{
		float PivotThresholder = EssentialStates.RotationMode == EMMAlsRotationMode::VelocityDirection ? 45.f : 30.f;
		FVector LastDir = EssentialValues.LastNonZeroVelocity.GetSafeNormal2D();
		FVector MoveDir = EssentialValues.CurrentAcceleration.GetSafeNormal2D();
		float TurnAngle = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(FVector::DotProduct(LastDir, MoveDir), -1.f, 1.f)));
		PivotStateTimer = TurnAngle >= PivotThresholder ? 
			AnimInstBase->PivotStateTimer : PivotStateTimer - AnimInstBase->GetDeltaSeconds();
		EssentialValues.bIsPivoting = PivotStateTimer > 0.f;
	}
	else
	{
		EssentialValues.bIsPivoting = false;
		PivotStateTimer = 0.f;
	}

	if (EssentialStates.bIsAiming)
	{
		FRotator RootBoneRot = Chr->GetMesh()->GetSocketRotation(TEXT("root"));
		RootBoneRot = UKismetMathLibrary::ComposeRotators(RootBoneRot, FRotator(0.f, 90.f, 0.f));
		float DeltaRotationYaw = UKismetMathLibrary::NormalizedDeltaRotator(Chr->GetActorRotation(), RootBoneRot).Yaw;
		EssentialValues.bTurnInPlace = FMath::Abs(DeltaRotationYaw) >= 45.f;
	}
	else
	{
		EssentialValues.bTurnInPlace = false;
	}
}

void FMMAlsAnimInstanceProxyBase::UpdateRagdollValues()
{
	// 涉及到物理状态的改变应先于动画层评估进行更新
	AnimInstBase->RagdollValues.bIsRagdolling = Chr->IsRagdolling();

	float PhysicsLinearSpeed = AnimInstBase->GetOwningComponent()->GetPhysicsLinearVelocity(TEXT("root")).Size();
	AnimInstBase->FlailRate = AnimInstBase->RagdollValues.bIsRagdolling ?
		0.f : FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1000.f), FVector2D(0.f, 1.f), PhysicsLinearSpeed);
}

void FMMAlsAnimInstanceProxyBase::UpdateLayeringCurves()
{
	const TMap<FName, float>& Curves = GetAnimationCurves(EAnimCurveType::AttributeCurve);

	static const auto GetCurveValue{
		[](const TMap<FName, float>& Curves, const FName& CurveName) -> float
		{
			const auto* Value{Curves.Find(CurveName)};

			return Value != nullptr ? *Value : 0.0f;
		}
	};

	LayeringCurves.ArmLeft = GetCurveValue(Curves, UMMAlsLayeringContants::LayerArmLeftCurveName());
	LayeringCurves.ArmRight = GetCurveValue(Curves, UMMAlsLayeringContants::LayerArmRightCurveName());
	LayeringCurves.ArmLeft_LS = GetCurveValue(Curves, UMMAlsLayeringContants::LayerArmLeftCurveName_LS());
	LayeringCurves.ArmRight_LS = GetCurveValue(Curves, UMMAlsLayeringContants::LayerArmRightCurveName_LS());

	// 设置手臂是否应在网格空间或局部空间中进行插值。 除非局部空间（LS）曲线完全加权，否则网格空间权重始终为1。
	LayeringCurves.ArmLeft_MS = 1 - UKismetMathLibrary::FFloor(LayeringCurves.ArmLeft_LS);
	LayeringCurves.ArmRight_MS = 1 - UKismetMathLibrary::FFloor(LayeringCurves.ArmRight_LS);

	LayeringCurves.HandLeft = GetCurveValue(Curves, UMMAlsLayeringContants::LayerHandLeftCurveName());
	LayeringCurves.HandRight = GetCurveValue(Curves, UMMAlsLayeringContants::LayerHandRightCurveName());

	LayeringCurves.EnableHandIK_L = GetCurveValue(Curves, UMMAlsLayeringContants::EnableHandLeftIkCurveName());
	LayeringCurves.EnableHandIK_R = GetCurveValue(Curves, UMMAlsLayeringContants::EnableHandRightIkCurveName());
}

void FMMAlsAnimInstanceProxyBase::UpdateLeanValue() const
{
	if (EssentialValues.bIsMoving)
	{
		FVector RelativeAcceleration = FVector::ZeroVector;
		if (EssentialValues.MaxAccelerationAmount > 0.f && EssentialValues.MaxDecelerationAmount > 0.f)
		{
			FVector VelocityDelta = (EssentialValues.Velocity - CachedEssentialValues.Velocity) / AnimInstBase->GetDeltaSeconds();
			if (FVector::DotProduct(EssentialValues.Velocity, EssentialValues.CurrentAcceleration) > 0.f)
			{
				FVector VelocityAcceleration =
					VelocityDelta.GetClampedToMaxSize(EssentialValues.MaxAccelerationAmount);
				RelativeAcceleration =
					Chr->GetActorRotation().UnrotateVector(VelocityAcceleration / EssentialValues.MaxAccelerationAmount);
			}
			else
			{
				FVector VelocityDeceleration =
					VelocityDelta.GetClampedToMaxSize(EssentialValues.MaxDecelerationAmount);
				RelativeAcceleration =
					Chr->GetActorRotation().UnrotateVector(VelocityDeceleration / EssentialValues.MaxDecelerationAmount);
			}
		}
		AnimInstBase->LeanValue = RelativeAcceleration.Y *
			FMath::GetMappedRangeValueClamped(FVector2D(200.f, 500.f), FVector2D(0.5f, 1.f), EssentialValues.Speed);
	}
	else
	{
		AnimInstBase->LeanValue = 0.f;
	}
}

void FMMAlsAnimInstanceProxyBase::UpdateAimOffset() const
{
	if (EssentialStates.RotationMode == EMMAlsRotationMode::LookingDirection)
	{
		FRotator AimRot = Chr->IsLocallyControlled() ? Chr->GetControlRotation() : Chr->GetBaseAimRotation();
		FRotator RootBoneRot = Chr->GetMesh()->GetSocketRotation(TEXT("root"));
		RootBoneRot = UKismetMathLibrary::ComposeRotators(RootBoneRot, FRotator(0.f, 90.f, 0.f));
		FRotator RotDelta = UKismetMathLibrary::NormalizedDeltaRotator(AimRot, RootBoneRot);
		FVector AO_Value = UKismetMathLibrary::VLerp(
			FVector(RotDelta.Yaw, RotDelta.Pitch, 0.f),
			FVector::ZeroVector,
			AnimInstBase->GetCurveValue(TEXT("Disable_AO"))
		);
		AnimInstBase->AimOffset.bEnableAO = FMath::Abs(AO_Value.X) <= (AnimInstBase->EssentialValues.bIsMoving ? 180.f : 115.f)
			&& AnimInstBase->GetSlotMontageLocalWeight(TEXT("DefaultSlot")) < 0.5f
			&& AnimInstBase->GetSlotMontageLocalWeight(TEXT("Traversal")) < 0.5f;
		AnimInstBase->AimOffset.AO_Yaw = AO_Value.X;
		AnimInstBase->AimOffset.AO_Pitch = AO_Value.Y;
		AnimInstBase->AimOffset.SpineRotation.Yaw = AnimInstBase->AimOffset.AO_Yaw / 6.f;	// 5层Spine + Pelvis
	}
	else
	{
		AnimInstBase->AimOffset.bEnableAO = false;
		AnimInstBase->AimOffset.SpineRotation.Yaw = 0.f;
	}
}

void FMMAlsAnimInstanceProxyBase::UpdateAimCamera() const
{
	if (EssentialStates.bIsAiming)
	{
		if (auto CameraComp = MoveComp->GetCameraComponent())
		{
			// 当 TurnInPlace 进入角度阈值内时开启 CameraAiming，另一种方案是手动添加 AnimNotify 控制 CameraAiming
			if (auto CameraAnimInst = Cast<UMMAlsCameraAnimInstance>(CameraComp->GetAnimInstance()))
			{
				if (!EssentialValues.bTurnInPlace && !CachedEssentialValues.bTurnInPlace &&
					!CameraAnimInst->GetActiveAiming() && CachedEssentialStates.bIsAiming)
				{
					MoveComp->OnGaitChangedDelegate.Broadcast(MoveComp->GetGait());
					CameraComp->OnActiveCameraAimingDelegate.Broadcast(true);
					if (CameraComp->GetShoulderMode() == EMMAlsShoulderMode::Left)
					{
						CameraComp->SetShoulderMode(EMMAlsShoulderMode::Left, true);
					}
					else
					{
						CameraComp->SetShoulderMode(EMMAlsShoulderMode::Right, true);
					}
				}
			}
		}
	}
	else
	{
		if (CachedEssentialStates.bIsAiming)
		{
			if (auto CameraComp = MoveComp->GetCameraComponent())
			{
				CameraComp->OnActiveCameraAimingDelegate.Broadcast(false);
				if (!CachedEssentialValues.bTurnInPlace)
				{
					CameraComp->SetShoulderMode(CameraComp->GetCameraSettings()->TP_Settings.LastShoulderMode);
				}
			}
		}
	}
}

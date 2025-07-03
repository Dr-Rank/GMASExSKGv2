// Fill out your copyright notice in the Description page of Project Settings.


#include "GMCE_MotionAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "PoseSearch/PoseSearchTrajectoryLibrary.h"

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Aiming, "State.IsAiming", "The player is aiming and should turn-in-place if needed.")

void UGMCE_MotionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetOwningComponent());
	AbilityComponent = Cast<UGMC_AbilitySystemComponent>(GetOwningActor()->GetComponentByClass(UGMC_AbilitySystemComponent::StaticClass()));
}

void UGMCE_MotionAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	// Save off this value before our parent updates it.
	WorldVelocityLastFrame = WorldVelocity;
	bWasMovingLastFrame = bIsMoving_MotionMatch;
	bHadPredictedStop = !bIsMoving_MotionMatch;
	bHadPredictedPivot = bIsPivoting_MotionMatch;
	MovementModeLastFrame = MovementMode;

	if (bIsFirstUpdate)
	{
		if (!IsValid(SkeletalMeshComponent))
		{
			SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetOwningComponent());
		}
		if (!IsValid(AbilityComponent))
		{
			AbilityComponent = Cast<UGMC_AbilitySystemComponent>(GetOwningActor()->GetComponentByClass(UGMC_AbilitySystemComponent::StaticClass()));
		}
	}
	
	Super::NativeUpdateAnimation(DeltaTime);

	WorldVelocityAcceleration = (WorldVelocityLastFrame - WorldVelocity) / FMath::Max(DeltaTime, 0.001);

	if (!WorldVelocity.IsNearlyZero())
	{
		LastNonZeroVelocity = WorldVelocity;
	}
	
	if (IsValid(MovementComponent))
	{
		MovementMode = MovementComponent->GetMovementMode();
		MaxInputAcceleration = MovementComponent->GetInputAcceleration();
		BrakingDeceleration = MovementComponent->GetBrakingDeceleration();
		LastLandingImpact = MovementComponent->LastLandingVelocity;

		if (MovementMode == EGMC_MovementMode::Grounded && MovementModeLastFrame == EGMC_MovementMode::Airborne)
		{
			bJustLanded = true;
			TimeSinceLastLanding = 0.f;
		}
		else
		{
			TimeSinceLastLanding += DeltaTime;
			bJustLanded = TimeSinceLastLanding < 0.3f;
		}
		
		// Copy out our predicted trajectory, allowing the automated conversion to kick in.
		auto Prediction = MovementComponent->PredictedTrajectory;
		if (Prediction.Samples.Num() > 0)
		{
			// Allow the GMCEx trajectory to auto-convert to the pose search one.
			// FPoseSearchQueryTrajectory TempTrajectory = Prediction;
			//
			// UPoseSearchTrajectoryLibrary::HandleTrajectoryWorldCollisions(this, this, TempTrajectory,
			// 	true, 0.01f, Trajectory, TrajectoryCollision,
			// 	UEngineTypes::ConvertToTraceType(ECC_Visibility), false, {}, EDrawDebugTrace::None);

			Trajectory = Prediction;

			const FGMCE_MovementSample CurrentSample = Prediction.GetSampleAtTime(-0.2f, false);
			const FGMCE_MovementSample NearFutureSample = Prediction.GetSampleAtTime(0.4f, false);
			const FGMCE_MovementSample FarFutureSample = Prediction.GetSampleAtTime(0.5f, false);
			FutureVelocity = (FarFutureSample.WorldTransform.GetLocation() - NearFutureSample.WorldTransform.GetLocation()) * 10.f;
			FutureFacing = FarFutureSample.WorldTransform.GetRotation();
			FutureGroundSpeed = FutureVelocity.Length();

			const FRotator FutureRotation = UKismetMathLibrary::Conv_VectorToRotator(FutureVelocity);
			const FRotator CurrentRotation = UKismetMathLibrary::Conv_VectorToRotator(CurrentSample.WorldLinearVelocity);
			const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(FutureRotation, CurrentRotation);

			// FVector PivotPoint;
			
			bIsMoving_MotionMatch = !WorldVelocity.Equals(FVector::ZeroVector, 0.1f) || !FutureVelocity.Equals(FVector::ZeroVector, 0.1f);
			bIsStarting_MotionMatch = FutureVelocity.Length() >= (WorldVelocity.Length() + 100.f) && bIsMoving_MotionMatch;
			bIsPivoting_MotionMatch = FMath::Abs(Delta.Yaw) >= MovementComponent->PivotPredictionAngleThreshold;
			bIsStopping_MotionMatch = FutureVelocity.IsNearlyZero() && FutureVelocity.Length() < WorldVelocity.Length() && !bIsPivoting_MotionMatch;

			bWasMovingRecently = Prediction.HasMovementInRange(-0.2f, 0.f);
			float OffsetYaw = 0.f;

			bool bIsAiming = false;

			if (IsValid(AbilityComponent))
			{
				bIsAiming = AbilityComponent->HasActiveTagExact(FGameplayTag::RequestGameplayTag("State.IsAiming"));
			}
			
			if (IsValid(SkeletalMeshComponent))
			{
				const FRotator RootRotation = SkeletalMeshComponent->GetBoneTransform(FName(TEXT("root"))).Rotator();
				const FRotator TargetRotation = CurrentSample.WorldTransform.GetRotation().Rotator();
				const FRotator MeshDelta = UKismetMathLibrary::NormalizedDeltaRotator(TargetRotation, RootRotation);
				
				OffsetYaw = FMath::Abs(MeshDelta.Yaw);
			}
			
			bool bOldTurnInPlace = bShouldTurnInPlace;
	
			float MinTurnOffset = (bOldTurnInPlace ? 5.f : (bIsAiming ? 40.f : 60.f));
			
			bShouldTurnInPlace = OffsetYaw >= MinTurnOffset && (bWasMovingLastFrame || bIsAiming || bOldTurnInPlace) && !bIsMoving_MotionMatch; 
		}
		else
		{
			FutureVelocity = FVector::ZeroVector;
			FutureFacing = FQuat::Identity;
			FutureGroundSpeed = 0.f;
			bIsStarting = false;
			Trajectory = FTransformTrajectory();
			bIsPivoting_MotionMatch = false;
			bIsStopping_MotionMatch = false;
		}
	}

}

FVector UGMCE_MotionAnimInstance::CalculateRelativeAccelerationAmount() const
{
	if (!IsValid(MovementComponent)) return FVector::ZeroVector;

	float AccelerationClamp = 0.f;
	if (FVector::DotProduct(WorldAcceleration, WorldVelocity) > 0.f)
	{
		AccelerationClamp = MaxInputAcceleration;
	}
	else
	{
		AccelerationClamp = MovementComponent->GetBrakingDeceleration();
	}

	// Get our acceleration relative to our maximum acceleration or braking deceleration, where 1 is max acceleration,
	// and -1 is max braking.
	const FVector ClampedAcceleration = WorldVelocityAcceleration.GetClampedToMaxSize(AccelerationClamp) / AccelerationClamp;

	// Return our acceleration relative to our current rotation.
	return OwnerTransform.GetRotation().UnrotateVector(ClampedAcceleration);
}


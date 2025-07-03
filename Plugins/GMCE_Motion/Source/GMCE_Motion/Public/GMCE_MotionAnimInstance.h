// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GMCAbilityComponent.h"
#include "GMCE_BaseAnimInstance.h"
#include "GMCE_OrganicMovementCmp.h"
#include "GMCOrganicMovementComponent.h"
#include "NativeGameplayTags.h"
#include "PoseSearch/PoseSearchTrajectoryLibrary.h"
#include "GMCE_MotionAnimInstance.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Aiming)

/**
 * 
 */
UCLASS()
class GMCE_MOTION_API UGMCE_MotionAnimInstance : public UGMCE_BaseAnimInstance
{
	GENERATED_BODY()

protected:

	/// Returns the current relative acceleration (since last frame), clamped to the movement component's
	/// maximum acceleration or deceleration.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GMCEx Motion Matching", meta=(BlueprintThreadSafe))
	FVector CalculateRelativeAccelerationAmount() const;

	/// Check whether we have just jumped (we are airborne but our previous mode was something else).
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GMCEx Motion Matching", meta=(BlueprintThreadSafe))
	bool JustJumped() const {
		return MovementMode == EGMC_MovementMode::Airborne &&
			MovementModeLastFrame != EGMC_MovementMode::Airborne;
	}

	/// Check whether we have just landed (our previous movement mode was airborne but our current movement mode is not).
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GMCEx Motion Matching", meta=(BlueprintThreadSafe))
	bool JustLanded() const
	{
		return bJustLanded;
	}

	bool bJustLanded { false };
	float TimeSinceLastLanding { 0.f };

	/// The force of our last landing.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	FVector LastLandingImpact { FVector::ZeroVector };
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	/// Our current input acceleration value, cached for thread safety.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	float MaxInputAcceleration;

	/// Our current braking deceleration value, cached for thread safety.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	float BrakingDeceleration;
	
	/// A cached copy of the current GMC movement mode.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	EGMC_MovementMode MovementMode;

	/// A cached copy of the previous GMC movement mode.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	EGMC_MovementMode MovementModeLastFrame;

	/// A cached copy of the previous Is Moving value.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	bool bWasMovingLastFrame;	
	
	/// A cached copy of the current character transform.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	FTransform OwnerTransform;
	
	/// A precalculated trajectory, suitable for passing into the Motion Match node without further change.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Motion Matching")
	FTransformTrajectory Trajectory;

	/// A precalculated near-future velocity.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	FVector FutureVelocity;

	/// A precalculated near-future direction we're facing.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	FQuat FutureFacing;

	/// A precalculated near-future ground speed.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	float FutureGroundSpeed;

	/// The velocity value from our previous animation frame.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	FVector WorldVelocityLastFrame;

	/// How quickly our velocity is changing, if anything, based not on the movement component but on our last
	/// recorded velocity.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	FVector WorldVelocityAcceleration;

	/// Our velocity the last time we moved at all.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	FVector LastNonZeroVelocity;
	
	/// A precalculated check on whether we're moving from an idle state into a movement state.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	bool bIsStarting { false };

	/// A precalculated check on whether we're moving from an idle state into a movement state.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	bool bHadPredictedStop { false };

	/// A precalculated check on whether we're moving from an idle state into a movement state.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	bool bHadPredictedPivot { false };
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching")
	bool bIsMoving_MotionMatch { false };
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching")
	bool bIsStarting_MotionMatch { false };
	
	/// A precalculated check on whether we're pivoting.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching")
	bool bIsPivoting_MotionMatch { false };

	/// A precalculated check on whether we're stopping.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching")
	bool bIsStopping_MotionMatch { false };

	/// A precalculated check on whether we stopped recently.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching")
	bool bWasMovingRecently { false };

	/// A precalculated check on whether should turn in place.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching")
	bool bShouldTurnInPlace { false };

	/// A precalculated check on whether should turn in place.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching")
	FTransform RootOffset { FTransform::Identity };
	
	/// A cached reference to our ability system component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching")
	UGMC_AbilitySystemComponent *AbilityComponent;
	
	/// The most recent collision results for our predicted trajectory.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion Matching")
	FPoseSearchTrajectory_WorldCollisionResults TrajectoryCollision;

	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;
};

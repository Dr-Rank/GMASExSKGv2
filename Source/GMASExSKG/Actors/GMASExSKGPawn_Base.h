#pragma once

#include "CoreMinimal.h"
#include "GMASExSKGMovementComponent.h"
#include "GMCAbilityComponent.h"
#include "GMCE_MotionWarpSubject.h"
#include "GMCE_OrganicMovementCmp.h"
#include "GMCPawn.h"
#include "InputMappingContext.h"
#include "GMASExSKGPawn_Base.generated.h"

/**
 * @class AGMASExSKGPawn_Base
 * @brief A class derived from AGMC_Pawn and implementing the IGMCE_MotionWarpSubject interface for advanced motion warping features.
 *
 * This class provides functionalities for handling advanced motion warping, player input setup, and essential pawn components.
 */
UCLASS()
class GMASEXSKG_API AGMASExSKGPawn_Base : public AGMC_Pawn, public IGMCE_MotionWarpSubject
{
	GENERATED_BODY()

public:
	AGMASExSKGPawn_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "GMCExTemplatePawn")
	virtual void Respawn();

	void Respawn_Internal();
	
	UFUNCTION(Client, Reliable)
	void CL_Respawn();

	UFUNCTION(Server, Reliable)
	void SV_Respawn();
	
	UFUNCTION()
	void OnRespawnCallback();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Respawn")
	void OnRespawn();
	
	// GMCExtendedAnimation motion warping interface
	virtual USkeletalMeshComponent* MotionWarping_GetMeshComponent() const override;
	virtual float MotionWarping_GetCollisionHalfHeight() const override;
	virtual FQuat MotionWarping_GetRotationOffset() const override;
	virtual FVector MotionWarping_GetTranslationOffset() const override;
	virtual FAnimMontageInstance* GetRootMotionAnimMontageInstance(USkeletalMeshComponent* MeshComponent) const override;
	virtual UGMCE_OrganicMovementCmp* GetGMCExMovementComponent() const override;

protected:

	// A convenience setting; if this is set, the given input mapping context will be added with
	// priority 0 automatically.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="Capsule", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UGMASExSKGMovementComponent> MovementComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UGMC_AbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<UGMCE_MotionWarpingComponent> MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	TObjectPtr<USkeletalMeshComponent> MotionWarpingMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Respawn")
	FGameplayTagContainer RespawnRemovesEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Respawn")
	FGameplayTagContainer RespawnRemovesActiveTags;
};

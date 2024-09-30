#include "GMASExSKGPawn.h"

#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GMASExSKG/Components/GMASExSKGMovementComponent.h"
#include "Utility/GMASUtilities.h"

AGMASExSKGPawn::AGMASExSKGPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	USceneComponent *TemporaryCapsule = GetComponentByClass<UCapsuleComponent>();

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	MeshComponent->bEditableWhenInherited = true;
	MeshComponent->SetupAttachment(TemporaryCapsule);
	MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	MeshComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
			
#if WITH_EDITOR
	UGMASUtilities::SetPropertyFlagsSafe(StaticClass(), TEXT("MeshComponent"), CPF_DisableEditOnInstance);

#endif	
}


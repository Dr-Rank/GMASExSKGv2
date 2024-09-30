#pragma once

#include "CoreMinimal.h"
#include "GMASExSKGMovementComponent.h"
#include "GMASExSKGPawn_Base.h"
#include "GMCE_MotionWarpingComponent.h"
#include "GMCE_MotionWarpSubject.h"
#include "Actors/GMAS_Pawn.h"
#include "UObject/Object.h"
#include "GMASExSKGPawn.generated.h"

class UGMASExSKGMovementComponent;

/**
 * @class AGMASExSKGPawn
 * @brief A pawn class that extends the base template pawn.
 *
 * This class serves as a third-person convenience pawn with additional components such as a skeletal mesh,
 * a spring arm, and a follow camera.
 */
UCLASS()
class GMASEXSKG_API AGMASExSKGPawn : public AGMASExSKGPawn_Base
{
	GENERATED_BODY()

public:
	explicit AGMASExSKGPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components", DisplayName="Skeletal Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
	
};

// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "C_StructsAndEnums.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "C_ANS_ContinuousAttackNotify.generated.h"

class ATOASCharacter;

/**
 * Anim Notify State to be used for attack animations that have multiple frames where attacks can be registered.
 * For example, sword swings.
 */
UCLASS()
class TOAS_API UC_ANS_ContinuousAttackNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	// The overridden NotifyBegin event called when the Anim Notify State has begun.
	// Sets the TOASCharacter using this Anim Notify State as the Attacker. 
	// It's not UFUNCTION() because it can be overridden by sub-classes of this Anim Notify State class.
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	// The overridden NotifyTick event called for as long as the Anim Notify State lasts before ending.
	// Sets the Locations from which the Attack must be traced, and call the proper function from the TOASCharacter
	// instance using this Anim Notify State, according to the AttackProperties.
	UFUNCTION()
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	// The overridden NotifyTick event called when the Anim Notify State reaches its end.
	// Removes the reference to the Attacker for good practices.
	UFUNCTION()
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	// A struct to organize and store universally recognized attack properties.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FAttackProperties AttackProperties;

	// Stores the reference to the attacker of this Anim Notify State for attacks.
	UPROPERTY()
	ATOASCharacter* Attacker;

	// Used to store the Location from where the Trace will begin.
	UPROPERTY()
	FVector StartLocation;
	
	// Used to store the Location from where the Trace will end.
	UPROPERTY()
	FVector EndLocation;
};

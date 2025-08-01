// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "C_ANS_ContinuousAttackNotify.h"
#include "C_StructsAndEnums.h"
#include "C_ANS_SolAttacks.generated.h"

/**
 * Specific Anim Notify State to be used by a specific character's attacks.
 */
UCLASS()
class TOAS_API UC_ANS_SolAttacks : public UC_ANS_ContinuousAttackNotify
{
	GENERATED_BODY()
protected:
	// Enumerator that defines from which hand the attack is going to happen.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sol_Attack_Properties")
	ESolHandAttack AttackFromHand;

	// Constant names for the recognized sockets in the character's mesh for weapon attacks.
	const FName LeftWeaponStart = "weapon_L";
	const FName LeftWeaponEnd = "weapon_End_L";
	const FName RightWeaponStart = "weapon_R";
	const FName RightWeaponEnd = "weapon_End_R";

public:
	/**
	 * The overridden NotifyBegin event called when the Anim Notify State has begun.
	 * From the beginning, overwrite the names of the sockets from where to trace the attack in the Attack Parameters,
	 * according to the AttackFromHand enum.
	 */
	UFUNCTION()
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
};

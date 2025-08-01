// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.


#include "C_ANS_SolAttacks.h"
#include "TOASCharacter.h"

void UC_ANS_SolAttacks::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// Only works if the Attacker's reference is valid.
	if (IsValid(Attacker) == true)
	{
		// Depending on the enum selected during the animation notify customization,
		// overwrite the AttackProperties with the intended socket names. 
		switch (AttackFromHand)
		{
		case ESolHandAttack::LEFT:
			AttackProperties.StartBoneName = LeftWeaponStart;
			AttackProperties.EndBoneName = LeftWeaponEnd;
			break;
		case ESolHandAttack::RIGHT:
			AttackProperties.StartBoneName = RightWeaponStart;
			AttackProperties.EndBoneName = RightWeaponEnd;
			break;
		}	
	}
}
// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.


#include "C_ANS_ContinuousAttackNotify.h"
#include "TOASCharacter.h"

void UC_ANS_ContinuousAttackNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	// Makes sure that the Mesh Component exists and is valid
	if (IsValid(MeshComp))
	{
		// Then the owner of said Mesh Component is casted as an ATOASCharacter class.
		if (ATOASCharacter* aThisAttacker = Cast<ATOASCharacter>(MeshComp->GetOwner()))
		{
			// And if the cast is successful, set it as the attacker of this notify.
			Attacker = aThisAttacker;
		}
	}
}

void UC_ANS_ContinuousAttackNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	// Is the Attacker has not been correctly registered in its reference, do not proceed with further code.
	if (IsValid(Attacker) == false)
	{
		return;
	}

	// Now, if the OverrideAttackLocation hasn't been modified,
	if (AttackProperties.OverrideAttackLocation == FVector::ZeroVector)
	{
		// as long as both StartBoneName and EndBoneName are distinct from "Default",
		// proceed to locate the respective sockets' world location. 
		if (AttackProperties.StartBoneName != FName("Default") && AttackProperties.EndBoneName != FName("Default"))
		{
			// Make sure the sockets do exist in the Skeletal Mesh to modify unset Location Parameters.
			if (Attacker->GetMesh()->DoesSocketExist(AttackProperties.StartBoneName))
			{
				StartLocation = Attacker->GetMesh()->GetSocketLocation(AttackProperties.StartBoneName);
			}
			if (Attacker->GetMesh()->DoesSocketExist(AttackProperties.EndBoneName))
			{
				EndLocation = Attacker->GetMesh()->GetSocketLocation(AttackProperties.EndBoneName);
			}
		}
		// Otherwise, set the StartLocation and EndLocation to the Attacker's Location.
		else
		{
			StartLocation = Attacker->GetActorLocation();
			EndLocation = Attacker->GetActorLocation();
		}
	}
	// However, if the AttackProperties has OverrideAttackLocation changed from Zeros,
	else
	{
		// then set every offset to their respective individual locations,
		FVector FwdLocation = Attacker->GetActorForwardVector() * AttackProperties.OverrideAttackLocation.Y;
		FVector SideLocation = Attacker->GetActorRightVector() * AttackProperties.OverrideAttackLocation.X;
		FVector UpLocation = Attacker->GetActorUpVector() * AttackProperties.OverrideAttackLocation.Z;
		// which will be joined together in one FVector value,
		FVector OverrideLocation = FwdLocation + SideLocation + UpLocation;
		// and finally, set to the StartLocation and EndLocation variables in relation to the Attacker's location.
		StartLocation = Attacker->GetActorLocation() + OverrideLocation;
		EndLocation = Attacker->GetActorLocation() + OverrideLocation;
	}
	// Finally, send the resulting Locations and Properties to the Attacker's function to Trace Attacks.
	Attacker->TraceAttack(StartLocation, EndLocation, AttackProperties);
}

void UC_ANS_ContinuousAttackNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	// When the Anim Notify State ends, release the attacker's pointer for good practices.
	Attacker = nullptr;
}

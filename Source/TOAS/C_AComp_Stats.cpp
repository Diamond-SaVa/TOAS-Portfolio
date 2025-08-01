// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.


#include "C_AComp_Stats.h"
#include "C_StructsAndEnums.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UC_AComp_Stats::UC_AComp_Stats()
{
	// This component does not need to Tick.
	PrimaryComponentTick.bCanEverTick = false;
}

void UC_AComp_Stats::AddEXP(int32 const &AddedEXP, bool& bLeveledUp)
{	
	// Adds EXP Points to the current pool of the Character's,
	// clamping it from 0 to 9 million 9 hundred and 99 thousand 9 hundred and 99 points.
	CurrentEXP = UKismetMathLibrary::FClamp(CurrentEXP + AddedEXP, 0, 9999999);
	// Returns the confirmation of Leveling Up when the obtained EXP surpasses the expected cap.
	bLeveledUp = CurrentEXP > GetEXPCapToLevelUp();
}

void UC_AComp_Stats::GetPhysicalDamage(const uint8& InstigatorATK, const float &fMultiplier, const EElementalAttribute& Element)
{
	// Calculates damage based on the subtraction of the instigator's ATK and the user's DEF.
	// Clamps damage to 1 in case it goes below ZERO.
	uint8 Damage = UKismetMathLibrary::Clamp(InstigatorATK - DEF, 1, 255);

	// Damage is affected by the multiplier variable coming from the attack for added effectiveness.
	Damage = UKismetMathLibrary::FFloor(Damage * fMultiplier);

	// Depending on the element
	switch (Element)
	{
	case EElementalAttribute::FIRE:
		Damage = UKismetMathLibrary::FFloor(Damage * (((101 - FireRES) * 1.0f) /100.0f));
		break;
	case EElementalAttribute::ICE:
		Damage = UKismetMathLibrary::FFloor(Damage * (((101 - IceRES) * 1.0f) /100.0f));
		break;
	case EElementalAttribute::THUNDER:
		Damage = UKismetMathLibrary::FFloor(Damage * (((101 - ThunderRES) * 1.0f) /100.0f));
		break;
	case EElementalAttribute::DARK:
		Damage = UKismetMathLibrary::FFloor(Damage * (((101 - DarkRES) * 1.0f) /100.0f));
		break;
	case EElementalAttribute::NON_LETHAL:
		// In the case of NON_LETHAL damage, said damage is reduced to ZERO.
		Damage = 0;
		break;
	default:
		break;
	}

	// Finally, subtract the Current HP by the final calculation of damage; clamping it to ZERO.
	CurrentHP = UKismetMathLibrary::Clamp(CurrentHP - Damage, 0, MaxHP);
}

// Called when the game starts
void UC_AComp_Stats::BeginPlay()
{
	Super::BeginPlay();

	// ...

	CurrentHP = MaxHP;
	
}
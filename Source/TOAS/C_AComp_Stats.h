// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "C_StructsAndEnums.h"
#include "Components/ActorComponent.h"
#include "C_AComp_Stats.generated.h"

/**
 * Actor Component that stores stats such as Levles, Experience, Attack, Defense, and also Resistances to Elements.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOAS_API UC_AComp_Stats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UC_AComp_Stats();

	// Getter for the Level of the Character.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	uint8 GetLevel() const { return Level; }
	
	// Getter of the currently obtained EXP of the Character
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	int32 GetEXP() const { return CurrentEXP; }
	
	// Caps the Experience to reach the next level based on the current Level before confirmation. 
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	int32 GetEXPCapToLevelUp() const { return (100 * GetLevel())-1; }
	
	/**
	 * Function called to add EXP to the pool of the Character's.
	 * Returns a boolean telling if the character leveled up after this increase.
	 * @param AddedEXP Experience Points to add to this Character's pool of EXP.
	 * @param bLeveledUp Checks whether the character has leveled up after having received Experience Points.
	 */
	UFUNCTION(BlueprintCallable, Category = "Stats_Setters")
	void AddEXP(int32 const &AddedEXP, bool &bLeveledUp);
	
	/**
	 * Getter of the currently possible Maximum amount of Hit Points. 
	 * @return MaxHP property.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	uint8 GetMaxHP() const { return MaxHP; }
	
	/**
	 * Getter of the current amount of Hit Points.
	 * @return CurrentHP property.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	uint8 GetCurrentHP() const { return CurrentHP; }

	/**
	 * Getter of the Attack points.
	 * @return ATK property.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	uint8 GetATK() const { return ATK; }

	/**
	 * Getter of the Defense points.
	 * @return DEF property.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	uint8 GetDEF() const { return DEF; }

	/**
	 * Getter of the Fire Resistance points.
	 * Reduces Damage from attacks with Fire properties.
	 * @return FireRES property.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	uint8 GetFireRES() const { return FireRES; }

	/**
	 * Getter of the Ice Resistance points.
	 * Reduces Damage from attacks with Ice properties.
	 * @return IceRES property.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	uint8 GetIceRES() const { return IceRES; }

	/**
	 * Getter of the Thunder Resistance points.
	 * Reduces Damage from attacks with Thunder properties.
	 * @return ThunderRES property.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	uint8 GetThunderRES() const { return ThunderRES; }

	/**
	 * Getter of the Darkness Resistance points.
	 * Reduces Damage from attacks with Darkness properties.
	 * @return DarkRES property.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats_Getters")
	uint8 GetDarkRES() const { return DarkRES; }

	/**
	 * Function that receives physical damage from physical weapons. Can be affected by the DEF Stat.
	 * @param InstigatorATK uint8 value that represents the ATK Stat of the Instigator who dealt damage.
	 * @param fMultiplier float value from instigator's power of their attack.
	 * @param Element Enumerator that may affect damage calculation based on its specific elemental resistance.
	 * Non-Lethal damage won't reduce HP.
	 */
	UFUNCTION(BlueprintCallable, Category = "Damage_Calculators")
	void GetPhysicalDamage(const uint8 &InstigatorATK, const float &fMultiplier, const EElementalAttribute &Element = EElementalAttribute::NEUTRAL );

private:
	// Current Level that determines a character's abilities and power. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	uint8 Level = 1;

	// Current amount of Experience accumulated by the character. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	int32 CurrentEXP = 0;

	// Max amount of Hit Points that the character will have.
	// Can be increased naturally through the progress of the story. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	uint8 MaxHP = 10;

	// Current amount of Hit Points that the keeps the character active and in action. Run out and it's game over. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	uint8 CurrentHP = 10;

	// Current amount of Attack power thar physical attacks will do. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	uint8 ATK = 5;

	// Current amount of Defense that will reduce damage from physical sources. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	uint8 DEF = 2;

	// Current amount of Resistance to fire based attacks. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	uint8 FireRES = 0;

	// Current amount of Resistance to ice based attacks. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	uint8 IceRES = 0;

	// Current amount of Resistance to electric based attacks. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	uint8 ThunderRES = 0;

	// Current amount of Resistance to darkness based attacks. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	uint8 DarkRES = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};

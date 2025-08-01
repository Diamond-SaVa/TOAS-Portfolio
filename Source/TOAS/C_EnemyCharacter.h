// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "TOASCharacter.h"
#include "C_EnemyCharacter.generated.h"

class UPawnSensingComponent;

/**
 * 
 */

// Delegation of Getting Damaged Event.
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDetection);

UCLASS()
class TOAS_API AC_EnemyCharacter : public ATOASCharacter
{
	GENERATED_BODY()

public:
	// Delegate for calling out to Damage Montages (using the preferable Blueprint Node with more control). 
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FPlayerDetection OnPlayerWasFound;

protected:
	// References to the grouped Attack Montage animation that is divided in different sections,
	// forming a chain of consecutive attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_Animations",
		meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	// References to the montage to play when enemy locates a player; mostly used for basic attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_Animations",
		meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FoundMontage;

	// Marked true if the enemy has found the player pawn.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy_Settings",
		meta = (AllowPrivateAccess = "true"))
	bool bCanFindPlayer = true;
	
	// Marked true if the enemy has found the player pawn.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy_Settings",
		meta = (AllowPrivateAccess = "true"))
	bool bPlayerWasFound = false;

	// Marked true if the enemy has found the player pawn.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy_Settings",
		meta = (AllowPrivateAccess = "true"))
	bool bInPursuit = false;

	// Marked true if the enemy has found the player pawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy_Settings",
		meta = (AllowPrivateAccess = "true"))
	FName SightOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy_Settings",
		meta = (AllowPrivateAccess = "true"))
	float SightDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy_Settings",
		meta = (AllowPrivateAccess = "true"))
	float SightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy_Settings",
		meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> SightTargetType;

public:
	AC_EnemyCharacter();

	virtual void Tick(float DeltaSeconds) override;

protected:
	void TraceForPlayer();
};

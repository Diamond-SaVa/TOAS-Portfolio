// Copyright Epic Games, Inc. All Rights Reserved.
// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Delegates/Delegate.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TOASCharacter.generated.h"

// Forward Declaration of following classes to be used:
// Stats Component.
class UC_AComp_Stats;
// Animation Montages to use.
class UAnimMontage;
// Widget Components for Z-Targeting System.
class UWidgetComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// Delegation of Getting Damaged Event.
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetDamagedEvent, float, LaunchImpulse);

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackLanded);

UCLASS(config=Game)
class ATOASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Returns the Stats Component for public access.
	FORCEINLINE UC_AComp_Stats* GetStats() const { return StatsComponent; }

	// Delegate for calling out to Damage Montages (using the preferable Blueprint Node with more control). 
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGetDamagedEvent OnGetDamagedEvent;

	// Delegate for calling out to when Attacks are a Hit (usually in the enemy side). 
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FAttackLanded OnAttackHasLanded;
	
protected:
	// Reference to the Stats Component.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character_Components",
		meta = (AllowPrivateAccess = "true"))
	UC_AComp_Stats* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character_Components")
	UWidgetComponent* TargetWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character_Components")
	UWidgetComponent* SeenWidgetComponent;

	// Actor to target and lock view onto.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PC_ControlValues", meta = (AllowPrivateAccess = "true"))
	ATOASCharacter* ZTargetToTrack;

	// Distance to the Locked On Target.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PC_ControlValues", meta = (AllowPrivateAccess = "true"))
	float DistanceToZTarget;
	
	// Check if this character is an enemy character or not.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_ControlValues",
		meta = (AllowPrivateAccess = "true"))
	bool bIsEnemy;
	
	// Check if this character is an NPC, interactable or not.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_ControlValues",
		meta = (AllowPrivateAccess = "true"))
	bool bIsNPC;

	// Check if the character can be hurt at the moment.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character_ControlValues",
		meta = (AllowPrivateAccess = "true"))
	bool bCanHurt;

	// Check if the character is in a state of Hurt.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character_ControlValues",
		meta = (AllowPrivateAccess = "true"))
	bool bIsHurt;

	// Timer to fill to reset bCanHurt value
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character_ControlValues",
		meta = (AllowPrivateAccess = "true"))
	float ResetHurt;

	// Timer threshold to surpass to reset bCanHurtValue
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character_ControlValues",
		meta = (AllowPrivateAccess = "true"))
	float ResetHurtSet = 0.3f;

	// Check if this character is an enemy character or not.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_ControlValues",
		meta = (AllowPrivateAccess = "true"))
	bool bIsKO;
	
	// References the montage to play when getting hurt.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_Animations",
		meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HurtMontage;

	// References the segmented montage(s) to play when character jumps, with variations.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_Animations",
		meta = (AllowPrivateAccess = "true"))
	UAnimMontage* JumpMontage;

	// References the montage(s) to play when landing on the ground, with variations.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_Animations",
		meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LandingMontage;

	// References setting for tracing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DEBUG",
		meta = (AllowPrivateAccess = "true"))
	 TEnumAsByte<EDrawDebugTrace::Type> DebugTraceEnum = EDrawDebugTrace::None;

public:
	// Constructor of the character.
	ATOASCharacter();

	// Tick for constant events, like timers and perspective managers. 
	virtual void Tick(float DeltaSeconds) override;

	// Function that uses Sphere Traces for Objects to track Hits from Attacks.
	// Can hurt opposing characters or even hit triggers like switches and other interactive Dynamic Actors in the world 
	UFUNCTION(BlueprintCallable, Category="CharacterFunctions")
	void TraceAttack(const FVector StartLocation, const FVector EndLocation, const FAttackProperties AttackProperties);

	// Function that uses Sphere Traces for Objects to track Hits from Attacks.
	// Can hurt opposing characters or even hit triggers like switches and other interactive Dynamic Actors in the world 
	UFUNCTION(BlueprintCallable, Category="CharacterFunctions")
	void TraceAttackMulti(const FVector StartLocation, const FVector EndLocation, const FAttackProperties AttackProperties);

	// Called when receiving damage from attacks or even hazards.
	// Works out the damage based on specific calculations, like elemental damage or Power Multiplier per Hit
	// (Multiplier obtained from each hit in an Animation).
	UFUNCTION(BlueprintCallable, Category="CharacterFunctions")
	void GettingDamaged(const uint8 &InstigatorATK, const float &fMultiplier, const FVector &InstigatorLocation,
		float FwdImpulse, float UpImpulse, const EElementalAttribute& ElementalAttribute);

	UFUNCTION(BlueprintCallable, Category="CharacterFunctions")
	void IsSeen();

	UFUNCTION(BlueprintCallable, Category="CharacterFunctions")
	void IsUnseen();

	UFUNCTION(BlueprintCallable, Category="CharacterFunctions")
	void IsTargeted();

protected:

	// Call begin play when spawning in the world.
	virtual void BeginPlay() override;

	// Obtains the distance from the Character to a set Location, for example, another Actor or even Character.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Calculators")
	void GetCharacterDistanceToLocation(const FVector TargetLocation, float &OutDistanceFloat);

	// Obtains the Rotator to face a specific target using only the location of said target.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Calculators")
	void GetLookAtRotatorWithLocation(const FVector TargetLocation, FRotator &rotOutRotation);

	// Obtains the Forward Vector of the Character times the Distance.
	// Negative Distance also applies for what's behind the Character.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Calculators")
	void GetActorForwardDistance(const float Distance, FVector& OutVector);

	// Obtains the Right Vector of the Character times the Distance.
	// Negative Distance also applies for what's to the Left of the Character.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Calculators")
	void GetActorRightDistance(const float Distance, FVector& OutVector);

	// Obtains the Up Vector of the Character times the Distance.
	// Negative Distance also applies for what's under the Character.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Calculators")
	void GetActorUpDistance(const float Distance, FVector& OutVector);

	// Obtains the Forward Vector relative to the Set Rotation in the parameters.
	// Negative Distance also applies for Backwards Vector.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Calculators")
	void GetForwardDistanceFromRotator(const float Distance, FRotator InRotation, FVector& OutVector);

	// Obtains the Right Vector relative to the Set Rotation in the parameters.
	// Negative Distance also applies for Left Vector.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Calculators")
	void GetRightDistanceFromRotator(const float Distance, FRotator InRotation, FVector& OutVector);

	// Obtains the Up Vector relative to the Set Rotation in the parameters.
	// Negative Distance also applies for Down Vector.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Calculators")
	void GetUpDistanceFromRotator(const float Distance, FRotator InRotation, FVector& OutVector);

	// Verifies if a specific float value is located in between two others.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Calculators")
	void CheckIfFloatIsInBetweenValues(const float& Distance, const float Min, const float Max, bool& IsInBetween);
};


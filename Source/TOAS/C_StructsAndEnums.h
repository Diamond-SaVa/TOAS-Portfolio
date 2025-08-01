// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "C_StructsAndEnums.generated.h"

class TOAS_API C_StructsAndEnums
{
public:
	C_StructsAndEnums();
	~C_StructsAndEnums();
};

USTRUCT(BlueprintType)
struct FSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checklists")
	TMap<FString, bool> ChallengesList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checklists")
	TMap<FString, bool> DialogueTriggersList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checklists")
	TMap<FString, bool> CutsceneList;
};

// Properties for Attack Traces called during Animations.
USTRUCT(BlueprintType)
struct FAttackProperties
{
	GENERATED_BODY()
	
	// Set it to the Name of the Bone in the Character Skeleton to track the Starting Location for the Sphere Trace.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties", meta=(AllowPrivateAccess=true))
	FName StartBoneName = FName("Default");
	
	// Set it to the Name of the Bone in the Character Skeleton to track the Ending Location for the Sphere Trace.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties", meta=(AllowPrivateAccess=true))
	FName EndBoneName = FName("Default");

	// Sets the Location Relative to the Character's Orientation to Trace the Attack, ignoring Bones to Locate.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties", meta=(AllowPrivateAccess=true))
	FVector OverrideAttackLocation = FVector::ZeroVector;

	// Sets the Location Relative to the Character's Orientation to Trace the Attack, ignoring Bones to Locate.
	// Automatically set to World Dynamic (for Triggers) and Pawn (for Characters).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties", meta=(AllowPrivateAccess=true))
	TArray<TEnumAsByte<EObjectTypeQuery>> HitObjectTypes =
		{ EObjectTypeQuery::ObjectTypeQuery2, EObjectTypeQuery::ObjectTypeQuery3 };
	
	// Sets the radius of the attack's Sphere Trace.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties", meta=(AllowPrivateAccess=true))
	float RadiusOfAttack = 20.0f;

	// Multiplier that affects the damage on Hit (from Trace).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties", meta=(AllowPrivateAccess=true))
	float AttackMultiplier = 1.0f;

	// How far will the enemy be pushed when Hit (from Trace).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties", meta=(AllowPrivateAccess=true))
	float AttackForwardImpulse = -500.0f;

	// How far will the enemy be raised when Hit (from Trace).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties", meta=(AllowPrivateAccess=true))
	float AttackUpImpulse = 100.0f;
};

// Used for AnimNoifies and AnimNotifyStates to check from which hand Sol will attack, using specific sockets/bones.
UENUM(BlueprintType)
enum class ESolHandAttack : uint8
{
	LEFT UMETA(DisplayName = "Left Hand"),
	RIGHT UMETA(DisplayName = "Right Hand")
};

// Used to determine the attribute an attack will have, which will have an effect on damage calculation (using RES).
UENUM(BlueprintType)
enum class EElementalAttribute : uint8
{
	NEUTRAL UMETA(DisplayName="Neutral Element"),
	FIRE UMETA(DisplayName = "Fire Element"),
	ICE UMETA(DisplayName = "Ice Element"),
	THUNDER UMETA(DisplayName = "Thunder Element"),
	DARK UMETA(DisplayName = "Dark Element"),
	NON_LETHAL UMETA(DisplayName = "Non-Lethal Element"),
};
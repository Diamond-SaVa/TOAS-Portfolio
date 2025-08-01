// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "TOASCharacter.h"
#include "C_PlayableCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USoundBase;
struct FInputActionValue;

// Enum used to verify if Z Targeting managed to locate it's Seen Target in Blueprint customization.
UENUM(BlueprintType)
enum class EZTargetResult : uint8
{
	Success UMETA(DisplayName = "Success"),
	Failure UMETA(DisplayName = "Failure"),
};

// Enum used to verify which type of attack behavior and animations to play in Blueprint customization.
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	// Route to use when input has been locked from further attacks.
	BlockedInput = 0 UMETA(DisplayName = "Input was Blocked"),
	// Route used to store the confirmation of the next attack.
	SaveAttack = 1 UMETA(DisplayName = "Save the Next Attack"),
	// Ground based behaviors and animations
	Ground = 2 UMETA(DisplayName = "Ground Attack"),
	// Airborn based behaviors and animations.
	Air = 3 UMETA(DisplayName = "Air Attack")
};

UCLASS()
class TOAS_API AC_PlayableCharacter : public ATOASCharacter
{
	GENERATED_BODY()

protected:
	/* Perspective Properties */
	// Used to lock perspective for certain platform challenges if true, overrides Control Rotation to this perspective.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	bool bIsLockedPerspective;

	// The rotator that stores where the actor should look at, be it a perspective of the level or an enemy.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	FRotator RotLockedPerspectiveRotator;

	// The rotator that stores where the actor should look at, be it a perspective of the level or an enemy.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	FRotator RotZTargetLookAtRotator;

	// Used to lock perspective towards the target tracked by the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	bool bLockOnZTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	float ZTraceRadius = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	float ZTraceDistance = 100.0f;

	// Actor to target and lock view onto.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PC_ControlValues", meta = (AllowPrivateAccess = "true"))
	ATOASCharacter* SeenTarget;

	// Distance to the currently Seen Target.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PC_ControlValues", meta = (AllowPrivateAccess = "true"))
	float DistanceToSeenTarget;
	
	// Offset to change the perspective of the camera when locking onto a target; Up or Down.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	float ZTargetUpOffset;
	// Offset to change the perspective of the camera when locking onto a target; Left or Right.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	float ZTargetRightOffset;
	// Offset to change the perspective of the camera when locking onto a target; Backward or Forward.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	float ZTargetFwdOffset;

	// Length of the Camera Boom to have when the perspective is locked.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	float LockedPerspectiveBoomLength = 750.0f;
	// Length of the Camera Boom to have when locking onto a target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	float ZTargetBoomLength = 600.0f;
	// Length of the Camera Boom to have normally.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	float NormalBoomLength = 400.0f;
	// Sets how fast or slow to transition Camera Boom Lengths.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerspectiveProperties", meta = (AllowPrivateAccess = "true"))
	float BoomLengthLerpMultiplier = 2.0f;
	
	/* Attack Properties */
	// The byte value of the current attack in execution.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackProperties", meta = (AllowPrivateAccess = "true"))
	uint8 AttackCount;

	// The byte value of the current maximum allowed attack chain.
	// Can be changed during runtime.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackProperties", meta = (AllowPrivateAccess = "true"))
	uint8 AttackCountMax;
	
	// Checks if the character is able to attack.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackProperties", meta = (AllowPrivateAccess = "true"))
	bool bCanDoAttacks;

	// Checks if the character is able to attack.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackProperties", meta = (AllowPrivateAccess = "true"))
	bool bCanDoSpecials;

	// Checks if the character will continue the chain of attacks.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackProperties", meta = (AllowPrivateAccess = "true"))
	bool bSaveTheAttack;

	// Checks if the character is currently in an attacking animation.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackProperties", meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	// Checks if the character is currently in an attacking animation while running.
	// This does not lock movement.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackProperties", meta = (AllowPrivateAccess = "true"))
	bool bIsRunAttacking;

	// Stores the Enum Byte value of the Pawns Object's ID for attack detection.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackProperties", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypePawn = { EObjectTypeQuery::ObjectTypeQuery3 };

	
	/* Control Properties */
	// The value of the Joystick in a Vector.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ControlProperties", meta = (AllowPrivateAccess = "true"))
	FVector StickVector;
	
	// The magnitude, or length, at which the Joystick is being pushed.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ControlProperties", meta = (AllowPrivateAccess = "true"))
	float StickMagnitude = 0.0f;

	// The customizable dead zone for player control.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ControlProperties", meta = (AllowPrivateAccess = "true"))
	float StickDeadZone = 0.2f;

	// The threshold that indicated when the user is walking or running.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ControlProperties", meta = (AllowPrivateAccess = "true"))
	float VelocityThreshold = 499.0f;

	// The Max Walk Speed before triggering running.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ControlProperties", meta = (AllowPrivateAccess = "true"))
	bool bSetToRunning = false;
	
	// The Max Walk Speed before triggering running.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ControlProperties", meta = (AllowPrivateAccess = "true"))
	float NormalMaxVelocity = 500.0f;
	
	// The Max Walk Speed after triggering running.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ControlProperties", meta = (AllowPrivateAccess = "true"))
	float RunningMaxVelocity = 1000.0f;
	
	/* Wall Slide Properties */
	// Checks if character can do a WallSlide, when it's moving while in the air.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WallSlideProperties", meta = (AllowPrivateAccess = "true"))
	bool bCanDoWallSlide;

	// Checks if the character IS sliding down a wall.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WallSlideProperties", meta = (AllowPrivateAccess = "true"))
	bool bIsWallSliding;

	// Checks if the character IS sliding down a wall.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WallSlideProperties", meta = (AllowPrivateAccess = "true"))
	bool bJustWallJumped;

	// The power of the Vertical Wall Jump.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallSlideProperties", meta = (AllowPrivateAccess = "true"))
	float WallJumpPowerUp = 850.0f;

	// The power of the Horizontal Wall Jump.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallSlideProperties", meta = (AllowPrivateAccess = "true"))
	float WallJumpPowerBack = 1000.0f;

	// The float value of how slow should the character fall while Wall Sliding;
	// 1.0 if character doesn't slide to 0 if character sticks to the wall.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallSlideProperties", meta = (AllowPrivateAccess = "true"))
	float WallSlideFallMultiplier = 0.5f;

	// Stores the Enum Byte value of the Static Objects' ID for wall detection.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "WallSlideProperties", meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypeWallSlide = { EObjectTypeQuery::ObjectTypeQuery1 };

	/* Dodge Properties */
	// Checks if the character can perform a dodge action.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DodgeProperties", meta = (AllowPrivateAccess = "true"))
	bool bCanDodge;

	// Checks if the character is currently in a dodge animation.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DodgeProperties", meta = (AllowPrivateAccess = "true"))
	bool bIsDodging;

	/* Exclusive Playable Character Animations */

	// References the montage(s) to play when dodging, with variations.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_Animations",
		meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage;

	// References to the grouped Attack Montage animation that is divided in different sections,
	// forming a chain of consecutive attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_Animations",
		meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	// References to the grouped Attack Montage animation that is played when moving around,
	// forming a chain of consecutive attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_Animations",
		meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMovingMontage;

	// Just like AttackMontage, this variable references the montage of attacks in the air,
	// also divided in different sections that form a chain of consecutive attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_Animations",
		meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackAirMontage;

	// Reference to the sound of stepping on ground.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_SFX",
		meta = (AllowPrivateAccess = "true"))
	USoundBase* Step_Dirt;
	
	// Reference to the sound of stepping on marble.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_SFX",
		meta = (AllowPrivateAccess = "true"))
	USoundBase* Step_Marble;

	// Reference to the sound of stepping on metal.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_SFX",
		meta = (AllowPrivateAccess = "true"))
	USoundBase* Step_Metal;

	// Reference to the sound of stepping on wood.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_SFX",
		meta = (AllowPrivateAccess = "true"))
	USoundBase* Step_Wood;

	// Reference to the sound of stepping on sand.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character_SFX",
		meta = (AllowPrivateAccess = "true"))
	USoundBase* Step_Sand;

	/* Components */
	// Camera boom positioning the camera behind the character.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// Follow camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	// MappingContext.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	// Jump Input Action.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// Move Input Action.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Look Input Action.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// Sprint Input Action.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

public:
	// Constructor
	AC_PlayableCharacter();

	// Tick
	virtual void Tick(float DeltaSeconds) override;

protected:
	// Begin Play; called once when actor spawns.
	virtual void BeginPlay() override;

	// Check when Character Controller has been changed.
	virtual void NotifyControllerChanged() override;

	// Setup Inputs and Actions; called once when actor spawns.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called for movement input.
	void MoveAct(const FInputActionValue& Value);
	
	// Called for movement input completed or interrupted.
	void StopMoveAct(const FInputActionValue& Value);

	// Called for looking input.
	void LookAct(const FInputActionValue& Value);

	// Called to begin a sprint.
	void SprintActStart(const FInputActionValue& Value);

	// Called to begin a sprint.
	void SprintActEnd(const FInputActionValue& Value);

	// Checks if Character can Wall Jump, and proceed to do so if they can.
	UFUNCTION(BlueprintCallable, Category = "PC_Functions")
	void WallJumpManager(bool &CouldJump);
	
	/**
	 * Manages Chain of Attacks and Booleans making sure that triggers won't be repeated.
	 * @param Branches Blueprint Output Nodes that lead to behaviors and animations to play
	 * based on context during execution.
	 */
	UFUNCTION(BlueprintCallable, Category="PC_Functions", meta=(ExpandEnumAsExecs = "Branches"))
	void AttackChainManager(EAttackType &Branches);
	
	// Automatically rotates character to face Z Target; mostly used as support for Attacks.
	UFUNCTION(BlueprintCallable, Category="PC_Functions")
	void RotateCharacterIfZTargetIsValid();

	// Automatically rotates character to a Location.
	UFUNCTION(BlueprintCallable, Category="PC_Functions")
	void RotateCharacterToWall(const FVector &WallHitLocation);

	// Manages when a character is able to Wall Slide, as long as conditions are met.
	// If the character is in the air, if the player is pushing the stick beyond the dead zone, and it's two traces
	// (from top and bottom of the character) meet with a wall marked as a World Static object,
	// then proceed to detect the sliding.
	UFUNCTION(BlueprintCallable, Category="PC_Functions")
	void WallSlideManager();

	// Manages the Tracing in search for a ZTarget to Track and/or See.
	UFUNCTION(BlueprintCallable, Category="PC_Functions")
	void TraceForZTarget();
	
	/**
	 * Obtain the valid ZTarget to Track if there is a Target being Seen already. 
	 * @param OutResult Output Execution Nodes that lead to how to make camera behave and rotate in blueprint.
	 */
	UFUNCTION(BlueprintCallable, Category="PC_Functions", meta=(ExpandEnumAsExecs = "OutResult"))
	void ObtainZTargetOrCamera(EZTargetResult& OutResult);
	
	/**
	 * Automatically rotates character to keep track of a perspective or a target.
	 * @param DeltaSeconds A pass-by reference that receives the DeltaSeconds from the Tick function.
	 */
	UFUNCTION(BlueprintCallable, Category="PC_Functions")
	void FixedCameraRotationManager(const float &DeltaSeconds);

	// Restores the Max Walk Speed to the expected value (if bSetToRunning is true or not).
	UFUNCTION(BlueprintCallable, Category="PC_Functions")
	void ReturnToTheExpectedSpeed();
	
	/**
	 * Restores the Max Walk Speed to the expected value (if bSetToRunning is true or not).
	 * @param SocketFoot Name of the socket located at the feet of the character from where to Trace for Sound.
	 * @param TraceRadius Radius of the Trace to check for walkable surface.
	 * @param OverrideSoundVolume Sends a new value to override the volume of the playing sound. Default is 1.00.
	 */
	UFUNCTION(BlueprintCallable, Category="PC_Functions")
	void TraceTheStepForSound(const FName& SocketFoot = FName("Default"),
		const float TraceRadius = 5.0f, const float OverrideSoundVolume = 1.0f);

	/* Inline functions for small and direct processes. */
	
	/**
	 * Sets the Perspective's Rotation and the Lock value.
	 * @param NewPerspective Rotator to set the Player Controller Rotation to make camera face a specific perspective.
	 * @param SetLock New boolean value to set bIsLockedPerspective in order to
	 * lock (true) or unlock (false) the perspective of the camera.
	 */
	UFUNCTION(BlueprintCallable, Category="PC_Functions")
	FORCEINLINE void ChangeCameraPerspectiveAndLock
	(const FRotator &NewPerspective = FRotator(0,0,0), const bool &SetLock = false)
	{
		RotLockedPerspectiveRotator = NewPerspective;

		bIsLockedPerspective = SetLock;
	}

public:
	// Returns CameraBoom sub-object.
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// Returns FollowCamera sub-object.
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
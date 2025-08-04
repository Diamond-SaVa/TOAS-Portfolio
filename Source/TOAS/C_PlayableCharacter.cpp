// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.


#include "C_PlayableCharacter.h"

#include "C_AComp_Stats.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


AC_PlayableCharacter::AC_PlayableCharacter()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AC_PlayableCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorld() && GetWorld()->IsPaused())
	{
		return;
	}
	// Allows the character to track when it faces a wall and begins to slide, or stick to it. 
	WallSlideManager();
	
	// Fixes the camera and the controller's orientation to the perspective it must follow,
	// depending on whether it's locked to a perspective of the level or targeting an Actor. 
	FixedCameraRotationManager(DeltaSeconds * BoomLengthLerpMultiplier);

	TraceForZTarget();
}

void AC_PlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set the following initial booleans that enable specific actions.
	bCanDoAttacks = true;
	bCanDodge = true;
	AttackCount = 0;

	
}

void AC_PlayableCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AC_PlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Moving Inputs
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AC_PlayableCharacter::MoveAct);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AC_PlayableCharacter::StopMoveAct);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &AC_PlayableCharacter::StopMoveAct);

		// Looking Inputs
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AC_PlayableCharacter::LookAct);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AC_PlayableCharacter::SprintActStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AC_PlayableCharacter::SprintActEnd);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AC_PlayableCharacter::SprintActEnd);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AC_PlayableCharacter::MoveAct(const FInputActionValue& Value)
{
	// Input is a Vector2D variable.
	const FVector2D MovementVector = Value.Get<FVector2D>();
	// Stored Vector of the Stick value.
	StickVector = FVector(MovementVector.X, MovementVector.Y, 0.0);
	// Magnitude of the Stick being pushed.
	StickMagnitude = MovementVector.Size();

	// If character is either Wall Sliding,
	// or the character does not possess a Player Controller,
	// or more specifically, if character is currently attacking a SeenTarget or a ZTarget,
	if (bIsWallSliding == true || Controller == nullptr
		|| (bIsAttacking == true && (SeenTarget != nullptr || ZTargetToTrack != nullptr)))
	{
		// then stop all further code execution.
		return;
	}

	// Find out which way is forward fom Controller Rotation,
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector from Controller Rotation,
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector from Controller Rotation,
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	// add movement using the vectors.
	AddMovementInput(ForwardDirection,  MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
	
}

void AC_PlayableCharacter::StopMoveAct(const FInputActionValue& Value)
{
	// Resets proper movement values to 0.
	StickMagnitude = 0.0f;
	
	StickVector = FVector(0.0f, 0.0f, 0.0f);

	StopAnimMontage(AttackMovingMontage);

	GetCharacterMovement()->MaxWalkSpeed = NormalMaxVelocity;
}

void AC_PlayableCharacter::LookAct(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	// If character lacks a Controller, or if the Perspective is Locked, or if there is a ZTarget being Tracked,
	if (Controller == nullptr || bIsLockedPerspective == true || IsValid(ZTargetToTrack) == true)
	{
		// then prevents further code execution from adding movement to camera from player input.
		return;
	}
	
	// add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AC_PlayableCharacter::SprintActStart(const FInputActionValue& Value)
{
	bSetToRunning = true;
	
	if (GetCharacterMovement() != nullptr)
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			GetCharacterMovement()->MaxWalkSpeed =  RunningMaxVelocity;	
		}
	}
}

void AC_PlayableCharacter::SprintActEnd(const FInputActionValue& Value)
{
	bSetToRunning = false;
	
	if (GetCharacterMovement() != nullptr)
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			GetCharacterMovement()->MaxWalkSpeed =  NormalMaxVelocity;	
		}
	}
}

void AC_PlayableCharacter::WallJumpManager(bool &CouldJump)
{
	// Automatically set CouldJump output to false in case no further code is to be executed.
	CouldJump = false;
	// If the character is not Wall Sliding,
	if (bIsWallSliding == false)
	{
		// then stop all further code execution and keep output to false.
		return;
	}

	// Creates both Vectors based on set jump power to create the wall jump effect.
	FVector JumpVector;
	GetActorForwardDistance(-WallJumpPowerBack, JumpVector);
	JumpVector.Z = WallJumpPowerUp;
	// Launch said character using the proper Vectors and its set power. 
	LaunchCharacter(JumpVector, true, true);

	// Automatically set rotation reverse to the character's current rotation.
	SetActorRotation(FRotator(0, GetActorRotation().Yaw + 180.0, 0));

	// Set the Output to confirm that Character was able to Wall Jump.
	CouldJump = true;
}

void AC_PlayableCharacter::AttackChainManager(EAttackType &Branches)
{
	// By default assign the output to BlockedInput. 
	Branches = EAttackType::BlockedInput;

	// If the character is hurt, is unable to attack, or is in the middle of dodging, by air or ground,
	if (bIsHurt == true || bCanDoAttacks == false || bIsDodging == true)
	{
		// block further code execution.
		return;
	}

	// If character is already attacking, reroute the output node to whatever is needed by the blueprint customization.
	if (bIsAttacking == true)
	{
		Branches = EAttackType::SaveAttack;
		return;
	}

	// Check if the character can perform attacks,
	if (bIsAttacking == false)
	{
		// Check if the character is on the ground, use AttackMontage.
		if (GetCharacterMovement()->IsMovingOnGround() == true)
		{
			Branches = EAttackType::Ground;
		}
		// However, if character is actually in the air,
		else
		{
			// Redirect the node on whether the contextual air attack applies or not.
			Branches = EAttackType::Air;
		}
	}
}

void AC_PlayableCharacter::RotateCharacterIfNotTargeting()
{
	// As long as there are no characters to ZTarget or See,
	if (IsValid(ZTargetToTrack) == true || IsValid(SeenTarget) == true ||
		GetController() == nullptr || StickVector == FVector::ZeroVector)
	{
		return;
	}
	
	// Find out which way is forward fom Controller Rotation,
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector from Controller Rotation,
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector from Controller Rotation,
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	FVector RotateDirection = (ForwardDirection * StickVector.Y) + (RightDirection * StickVector.X);
	RotateDirection.Normalize();

	SetActorRotation(RotateDirection.ToOrientationRotator());
}

void AC_PlayableCharacter::RotateCharacterToWall(const FVector& WallHitLocation)
{
	// Make sure to get the rotation that the target is facing.
	const FRotator RotFromTarget = UKismetMathLibrary::MakeRotFromX(WallHitLocation);
	// With that, calculate the new rotation by using only the Yaw value of the target to rotate.
	const FRotator TargetRot = FRotator(0.0, RotFromTarget.Yaw + 180.0, 0.0);
	SetActorRotation(TargetRot);
}

void AC_PlayableCharacter::WallSlideManager()
{
	// Checks if character can slide down a wall if it's not on the ground AND the player is pushing the joystick.
	bCanDoWallSlide = GetCharacterMovement()->IsMovingOnGround() == false && StickMagnitude > StickDeadZone;

	// If it can do a Wall Slide, or if the character is Hurt,
	if (bCanDoWallSlide == false || bIsHurt == true || bIsAttacking == true || bJustWallJumped == true
	|| GetController() == nullptr)
	{
		// and if the character was Wall Sliding previously,
		if (bIsWallSliding == true)
		{
			// then mark it as not sliding anymore.
			bIsWallSliding = false;
		}
		// As long as the character is unable to Wall Slide, stop all further code execution.
		return;
	}

	// Creates an FHitResult variable to store important variables for wall detection.
	FHitResult Hit;
	
	// Creates a Vector variable setting how far and forward should the Trace reach
	// according to the current Player Controller Rotation.
	FVector FwdAimingTrace;
	GetForwardDistanceFromRotator(60.0f, GetController()->GetControlRotation(), FwdAimingTrace);
	
	FVector SideAimingTrace;
	GetRightDistanceFromRotator(60.0f, GetController()->GetControlRotation(), SideAimingTrace);

	FVector TraceDistance = (FwdAimingTrace * StickVector.Y) + (SideAimingTrace * StickVector.X);

	// Creates a Vector variable setting how high (or low when subtracting) the Trace should be.
	FVector UpOffset;
	GetActorUpDistance(55.0f, UpOffset);

	// Creates a boolean to detect if the upper part of the character's body is facing a wall.
	bool UpperBodyTrace = UKismetSystemLibrary::LineTraceSingleForObjects(this, GetActorLocation() + UpOffset,
		GetActorLocation() + TraceDistance + UpOffset, ObjTypeWallSlide, false, {},
		DebugTraceEnum, Hit, true);

	// Creates a boolean to detect if the lower part of the character's body is facing a wall.
	bool LowerBodyTrace = UKismetSystemLibrary::LineTraceSingleForObjects(this, GetActorLocation() - UpOffset,
		GetActorLocation() + TraceDistance - UpOffset, ObjTypeWallSlide, false, {},
		DebugTraceEnum, Hit, true);

	// Confirm that the character is Wall Sliding when both conditions apply.
	bIsWallSliding = UpperBodyTrace && LowerBodyTrace;
		
	// If it's confirmed that character is Wall Sliding,
	if (bIsWallSliding == true)
	{
		// Make character rotate towards the normal hit position of the wall.
		RotateCharacterToWall(Hit.Normal);
		// Modify the X and Y velocity of the character, and slowdown the character's Z velocity.
		GetCharacterMovement()->Velocity = FVector(GetCharacterMovement()->Velocity.X,
			GetCharacterMovement()->Velocity.Y, GetCharacterMovement()->Velocity.Z * WallSlideFallMultiplier);

		// If there are animations being played as Montages,
		if(GetMesh()->GetAnimInstance()->MontageInstances.Num() > 0)
		{
			// then stop all other Montages being played.
			StopAnimMontage();
		}
	}
}

void AC_PlayableCharacter::TraceForZTarget()
{
	// If the character is an enemy or is locking on, stop the function immediately.
	if (bLockOnZTarget == true)
	{
		return;
	}

	if (ZTargetToTrack != nullptr)
	{
		DistanceToZTarget = GetDistanceTo(ZTargetToTrack);
	}

	if (SeenTarget != nullptr)
	{
		DistanceToSeenTarget = GetDistanceTo(SeenTarget);
	}

	// Creates the array where to register the results from the trace.
	TArray<FHitResult> Hit;

	// Creates a Rotator that works from the camera's perspective, only using the Z/Yaw value.
	FRotator ControlZRotator = FRotator();
	
	if (GetController() != nullptr)
	{
		ControlZRotator = GetController()->GetControlRotation();
		ControlZRotator.Pitch = 0.0;
		ControlZRotator.Roll = 0.0;
	}

	// Creates a Vector that measures the forward distance from the ControlZRotator perspective.
	FVector OffsetVector;
	GetForwardDistanceFromRotator(ZTraceDistance, ControlZRotator, OffsetVector);

	// Creates the target location where the Trace will end. 
	const FVector TargetLocation = GetActorLocation() + OffsetVector;

	// Boolean checking if there was a hit from the Trace, using previously created variables.
	const bool WasHit = UKismetSystemLibrary::SphereTraceMultiForObjects(this, GetTargetLocation(),
		TargetLocation, ZTraceRadius, ObjTypePawn, false, {},
		DebugTraceEnum, Hit, true);

	// If there was a hit, proceed with the following instructions.
	if (WasHit == true)
	{
		// Check if there is a Target already being tracked.
		if (ZTargetToTrack != nullptr)
		{
			// IF there are more than 1 characters being Traced,
			if (Hit.Num() > 1)
			{
				// If the Tracked Target is not the same as the first actor in the array of results,
				// do the following to make said first actor the Seen Target. 
				if (ZTargetToTrack != Hit[0].GetActor())
				{
					// If the Seen Target is not the same as the first actor in the array as well,
					if (SeenTarget != Hit[0].GetActor())
					{
						// and if there is already another Target being Seen,
						if (SeenTarget != nullptr)
						{
							// make it Unseen.
							SeenTarget->IsUnseen();
						}

						// Cast the first actor in the array as the Seen Target.
						SeenTarget = Cast<ATOASCharacter>(Hit[0].GetActor());

						// If the cast was a success and there is a valid character in the variable,
						if (SeenTarget != nullptr)
						{
							// and if said character's HP is greater than 0,
							if (SeenTarget->GetStats()->GetCurrentHP() > 0)
							{
								// mark it as Seen.
								SeenTarget->IsSeen();
							}
						}
					}

					// Once done, stop code execution.
					return;
				}

				// Now, if the Seen Target is not the same as the second actor in the array,
				if (SeenTarget != Hit[1].GetActor())
				{
					// if there was already a Seen Target,
					if (SeenTarget != nullptr)
					{
						// mark it as unseen.
						SeenTarget->IsUnseen();
					}

					// Cast the second actor as the Seen Target.
					SeenTarget = Cast<ATOASCharacter>(Hit[1].GetActor());

					// If the cast was a success,
					if (SeenTarget != nullptr)
					{
						// and if the character's HP is greater than 0,
						if (SeenTarget->GetStats()->GetCurrentHP() > 0)
						{
							// mark it as Seen.
							SeenTarget->IsSeen();
						}
					}

					// Once done, stop code execution.
					return;
				}
			}
			// However, if the number of traced actors in the array is only one, it means that there should not be
			// another Target to be Seen.
			else
			{
				// If there is a valid Seen Target,
				if (SeenTarget != nullptr)
				{
					// mark it as unseen,
					SeenTarget->IsUnseen();
					// and empty its reference.
					SeenTarget = nullptr;
				}
			}

			// Check on each character traced and make sure if the currently Target being Tracked,
			for (FHitResult& HitResult : Hit)
			{
				// if said Target is the same as the Actor traced,
				if (ZTargetToTrack == HitResult.GetActor())
				{
					// stop further code execution.
					return;
				}
			}
			// Otherwise, if the Target is not found within the array,
			// then mark it as Unseen, and therefore not targeted.
			ZTargetToTrack->IsUnseen();
			// Also, remove the target's reference from the variable.
			ZTargetToTrack = nullptr;
			return;
		}

		// Otherwise, if there is no Locked Target, and
		// if the Seen Target is not the same as the first traced actor in the array,
		// then do the following to make said traced actor the Seen Target.
		if (SeenTarget != Hit[0].GetActor())
		{
			// If there was already another Seen Target,
			if (SeenTarget != nullptr)
			{
				// mark it as Unseen.
				SeenTarget->IsUnseen();
			}

			// Cast the first actor of the array as the Seen Target.
			SeenTarget = Cast<ATOASCharacter>(Hit[0].GetActor());

			// If the cast was a success,
			if (SeenTarget != nullptr)
			{
				// and if the Seen Target is greater than 0,
				if (SeenTarget->GetStats()->GetCurrentHP() > 0)
				{
					// mark it as Seen.
					SeenTarget->IsSeen();
				}
			}

			// Once that is done, stop further code execution.
			return;
		}

		// As long as the Trace detects a hit, stop further code execution.
		return;
	}

	// From now on, no actor is detected through the Trace.
	// If there was previously a detected Seen Target,
	if (SeenTarget != nullptr)
	{
		// mark it as Unseen,
		SeenTarget->IsUnseen();
		// and remove its reference from the variable.
		SeenTarget = nullptr;
	}
	// If there was a Target being Tracked,
	if (ZTargetToTrack != nullptr)
	{
		// mark it as Unseen, and therefor, untracked. 
		ZTargetToTrack->IsUnseen();
		// and then remove its reference from the variable.
		ZTargetToTrack = nullptr;
	}
}

void AC_PlayableCharacter::ObtainZTargetOrCamera(EZTargetResult& OutResult)
{
	// If there is not a target being seen at the moment, do not continue with the execution of the code.
	if (SeenTarget == nullptr)
	{
		// And redirect the blueprint towards the Failure Node Exit.
		OutResult = EZTargetResult::Failure;
		return;
	}

	// However, if there is a Seen Target, but also a registered ZTarget being tracked,
	if (ZTargetToTrack != nullptr)
	{
		// mark the current Target being Tracked as Unseen, and therefor, not targeted,
		ZTargetToTrack->IsUnseen();
		// and also remove the current tracked Target.
		ZTargetToTrack = nullptr;
	}

	// As long as the target being seen is alive,
	if (SeenTarget->GetStats()->GetCurrentHP() > 0)
	{
		// make that Seen Target the current Target to Track.
		ZTargetToTrack = SeenTarget;
		// and also empty the Seen Target reference.
		SeenTarget = nullptr;
		// The new Target being Tracked 
		ZTargetToTrack->IsTargeted();
		// Also redirect the blueprint towards the Success Node Exit.
		OutResult = EZTargetResult::Success;

		return;
	}
	// Otherwise, if nothing happens, then redirect the blueprint towards the Failure Node Exit. 
	OutResult = EZTargetResult::Failure;
}

void AC_PlayableCharacter::FixedCameraRotationManager(const float &DeltaSeconds)
{
	// Only when the perspective must be locked,
	if (bIsLockedPerspective == true)
	{
		GetCameraBoom()->TargetArmLength =
			UKismetMathLibrary::Lerp(GetCameraBoom()->TargetArmLength, LockedPerspectiveBoomLength, DeltaSeconds);
		
		// interpolates to the Perspective the camera must face.
		const FRotator LerpRot = UKismetMathLibrary::RLerp(
			GetController()->GetControlRotation(), RotLockedPerspectiveRotator, DeltaSeconds, true
		);
		// Use that interpolated rotation to control orientation and camera perspective.
		GetController()->SetControlRotation(LerpRot);

		// Makes sure that it stays locked by not enabling further modifications, even when there is a valid Z Target.
		return;
	}

	// Now, only if there is a valid Target being referenced,
	if (IsValid(ZTargetToTrack) == true)
	{
		GetCameraBoom()->TargetArmLength =
			UKismetMathLibrary::Lerp(GetCameraBoom()->TargetArmLength, ZTargetBoomLength, DeltaSeconds);

		// Get all Vector and Rotator information to track where to point and match camera angle towards the target.
		FRotator RotatorFromTarget;
		GetLookAtRotatorWithLocation(ZTargetToTrack->GetActorLocation(), RotatorFromTarget);

		FVector ForwardDistance;
		GetForwardDistanceFromRotator(ZTargetFwdOffset, RotatorFromTarget, ForwardDistance);
		
		FVector RightDistance;
		GetRightDistanceFromRotator(ZTargetRightOffset, RotatorFromTarget, RightDistance);
		
		FVector UpDistance;
		GetUpDistanceFromRotator(ZTargetUpOffset, RotatorFromTarget, UpDistance);

		const FVector StartLocation = GetActorLocation() + ForwardDistance + RightDistance + UpDistance;
		
		RotZTargetLookAtRotator = UKismetMathLibrary::FindLookAtRotation(StartLocation, ZTargetToTrack->GetActorLocation());
		RotZTargetLookAtRotator.Pitch = 0.0;
		
		// interpolate the Rotation to its Location.
		const FRotator LerpRot = UKismetMathLibrary::RLerp(
			GetController()->GetControlRotation(), RotZTargetLookAtRotator, DeltaSeconds, true
		);
		// Use that interpolated rotation to control orientation and camera perspective.
		GetController()->SetControlRotation(LerpRot);

		return;
	}

	// Otherwise, when the perspective is not locked, and neither there is a target to track,
	if (GetCharacterMovement() != nullptr)
	{
		// According to the movement, set the camera boom distance according to how fast it's going.
		// Take the lerp determined by the velocity of the character,
		float Lerp = GetCharacterMovement()->Velocity.Size2D();
		// prepare it for decimal value (0.0 to 1.0),
		Lerp /= RunningMaxVelocity;

		// and use to set up the next distance to lerp into.
		float LerpLength = UKismetMathLibrary::Lerp(NormalBoomLength, ZTargetBoomLength, Lerp);

		// And said distance smoothly lerp with the Delta Time received.
		GetCameraBoom()->TargetArmLength = UKismetMathLibrary::Lerp(GetCameraBoom()->TargetArmLength, LerpLength, DeltaSeconds);
	}
}

void AC_PlayableCharacter::ReturnToTheExpectedSpeed()
{
	// As long as there is a character movement component,
	if (GetCharacterMovement() != nullptr)
	{
		// if the button to run/sprint is being held, then update the max walking speed to the running max velocity.
		if (bSetToRunning == true)
		{
			GetCharacterMovement()->MaxWalkSpeed = RunningMaxVelocity;
		}
		// Otherwise, if the button is not being pressed, then update the max walking speed to the normal max velocity.
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = NormalMaxVelocity;
		}
	}
}

void AC_PlayableCharacter::TraceTheStepForSound(const FName& SocketFoot, const float TraceRadius,
	const float OverrideSoundVolume)
{
	if (GetMesh()->DoesSocketExist(SocketFoot))
	{
		FHitResult Hit;
		FVector SocketLocation = GetMesh()->GetSocketLocation(SocketFoot);
		bool STEP = UKismetSystemLibrary::SphereTraceSingle(this, SocketLocation,
			SocketLocation, TraceRadius, TraceTypeQuery1, true, {},
			DebugTraceEnum, Hit, true);

		if (STEP == true)
		{
			if (Hit.GetActor()->ActorHasTag(FName("Dirt")))
			{
				UGameplayStatics::SpawnSoundAtLocation(this, Step_Dirt, SocketLocation,
					GetActorRotation(), OverrideSoundVolume);
				return;
			}

			if (Hit.GetActor()->ActorHasTag(FName("Marble")))
			{
				UGameplayStatics::SpawnSoundAtLocation(this, Step_Marble, SocketLocation,
					GetActorRotation(), OverrideSoundVolume);
				return;
			}
			
			if (Hit.GetActor()->ActorHasTag(FName("Metal")))
			{
				UGameplayStatics::SpawnSoundAtLocation(this, Step_Metal, SocketLocation,
					GetActorRotation(), OverrideSoundVolume);
				return;
			}

			if (Hit.GetActor()->ActorHasTag(FName("Wood")))
			{
				UGameplayStatics::SpawnSoundAtLocation(this, Step_Wood, SocketLocation,
					GetActorRotation(), OverrideSoundVolume);
				return;
			}

			if (Hit.GetActor()->ActorHasTag(FName("Sand")))
			{
				UGameplayStatics::SpawnSoundAtLocation(this, Step_Sand, SocketLocation,
					GetActorRotation(), OverrideSoundVolume);
				return;
			}
		}
	}
}

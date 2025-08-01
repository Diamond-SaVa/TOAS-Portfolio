// Copyright Epic Games, Inc. All Rights Reserved.
// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#include "TOASCharacter.h"
#include "C_StructsAndEnums.h"
#include "C_AComp_Stats.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATOASCharacter

ATOASCharacter::ATOASCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create the Actor Component to track stats and levels.
	StatsComponent = CreateDefaultSubobject<UC_AComp_Stats>("Character_Statics");
	// Creates and sets the Widget Component to display the target system.
	TargetWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Target_WidgetComponent");
	TargetWidgetComponent->AttachToComponent(GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform);
	TargetWidgetComponent->SetDrawSize(FVector2D::One() * 50.0f);

	SeenWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Seen_WidgetComponent");
	SeenWidgetComponent->AttachToComponent(GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform);
	SeenWidgetComponent->SetDrawSize(FVector2D::One() * 50.0f);

	bCanHurt = true;
}

void ATOASCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ResetHurt < ResetHurtSet)
	{
		ResetHurt += DeltaSeconds;
	}
	else if (ResetHurt > ResetHurtSet)
	{
		bCanHurt = true;
		ResetHurt = ResetHurtSet;
	}
}

void ATOASCharacter::GetCharacterDistanceToLocation(const FVector TargetLocation, float& OutDistanceFloat)
{
	OutDistanceFloat = FVector::Distance(GetActorLocation(), TargetLocation);
}

void ATOASCharacter::GetLookAtRotatorWithLocation(const FVector TargetLocation, FRotator &OutRotation)
{
	OutRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
}

void ATOASCharacter::GetActorForwardDistance(const float Distance, FVector& OutVector)
{
	OutVector = GetActorForwardVector() * Distance;
}

void ATOASCharacter::GetActorRightDistance(const float Distance, FVector& OutVector)
{
	OutVector = GetActorRightVector() * Distance;
}

void ATOASCharacter::GetActorUpDistance(const float Distance, FVector& OutVector)
{
	OutVector = GetActorUpVector() * Distance;
}

void ATOASCharacter::GetForwardDistanceFromRotator(const float Distance, const FRotator InRotation, FVector& OutVector)
{
	OutVector = UKismetMathLibrary::GetForwardVector(InRotation) * Distance;
}

void ATOASCharacter::GetRightDistanceFromRotator(const float Distance, const FRotator InRotation, FVector& OutVector)
{
	OutVector = UKismetMathLibrary::GetRightVector(InRotation) * Distance;
}

void ATOASCharacter::GetUpDistanceFromRotator(const float Distance, const FRotator InRotation, FVector& OutVector)
{
	OutVector = UKismetMathLibrary::GetUpVector(InRotation) * Distance;
}

void ATOASCharacter::CheckIfFloatIsInBetweenValues(const float& Distance, const float Min, const float Max, bool& IsInBetween)
{
	IsInBetween = (Distance >= Min && Distance <= Max);
}

void ATOASCharacter::TraceAttack(const FVector StartLocation, const FVector EndLocation, const FAttackProperties AttackProperties)
{
	// This array is created here to make Tracing work, despite not having anything here.
	TArray<AActor*> ActorsToIgnore = {};

	// Used to store the FHitResult that is returned from the Tracing.
	FHitResult HitResults;

	// Performs the Trace using FVectors received from the Anim Notify or Anim Notify State
	bool HIT = UKismetSystemLibrary::SphereTraceSingleForObjects(this, StartLocation, EndLocation,
		AttackProperties.RadiusOfAttack, AttackProperties.HitObjectTypes, false, ActorsToIgnore,
		DebugTraceEnum, HitResults, true, FLinearColor::Red,
		FLinearColor::Green, 0.25f);

	if (HIT == false)
	{
		// If no hit was detected, stop function execution.
		return;
	}

	if (ATOASCharacter* CastedChar = Cast<ATOASCharacter>(HitResults.GetActor()))
	{
		if (CastedChar->bIsEnemy != bIsEnemy)
		{
			// Get the Attack stat from this character's Stats Component
			// as well as the Attack Properties coming from the animation.
			CastedChar->GettingDamaged(GetStats()->GetATK(), AttackProperties.AttackMultiplier, GetActorLocation(),
				AttackProperties.AttackForwardImpulse, AttackProperties.AttackUpImpulse,
				EElementalAttribute::NEUTRAL);
			// Stop function execution when an attack landed on a Character, be it player or enemy.
			// Hits should prioritize Characters.

			if (CastedChar == ZTargetToTrack)
			{
				if (CastedChar->GetStats()->GetCurrentHP() <= 0)
				{
					ZTargetToTrack = nullptr;
				}
			}

			OnAttackHasLanded.Broadcast();
		}
		return;
	}

	// Section reserved for Interactable Objects on Hit.
}

void ATOASCharacter::TraceAttackMulti(const FVector StartLocation, const FVector EndLocation,
	const FAttackProperties AttackProperties)
{
	// This array is created here to make Tracing work, despite not having anything here.
	TArray<AActor*> ActorsToIgnore = {};

	// Used to store the FHitResult that is returned from the Tracing.
	TArray<FHitResult> HitResults;

	// Performs the Trace using FVectors received from the Anim Notify or Anim Notify State
	bool HIT = UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartLocation, EndLocation,
		AttackProperties.RadiusOfAttack, AttackProperties.HitObjectTypes, false, ActorsToIgnore,
		DebugTraceEnum, HitResults, true);

	if (HIT == false)
	{
		// If no hit was detected, stop function execution.
		return;
	}

	for (FHitResult HitResult : HitResults)
	{
		if (ATOASCharacter* CastedChar = Cast<ATOASCharacter>(HitResult.GetActor()))
		{
			if (CastedChar->bIsEnemy != bIsEnemy)
			{
				// Get the Attack stat from this character's Stats Component
				// as well as the Attack Properties coming from the animation.
				CastedChar->GettingDamaged(GetStats()->GetATK(), AttackProperties.AttackMultiplier, GetActorLocation(),
					AttackProperties.AttackForwardImpulse, AttackProperties.AttackUpImpulse,
					EElementalAttribute::NEUTRAL);
				// Stop function execution when an attack landed on a Character, be it player or enemy.
				// Hits should prioritize Characters.

				if (CastedChar == ZTargetToTrack)
				{
					if (CastedChar->GetStats()->GetCurrentHP() <= 0)
					{
						ZTargetToTrack = nullptr;
					}
				}
			}
		}

		// Reserved for interactable objects on Hit.
	}
}

void ATOASCharacter::GettingDamaged(const uint8 &InstigatorATK, const float &fMultiplier,
                                    const FVector &InstigatorLocation, float FwdImpulse, float UpImpulse,
                                    const EElementalAttribute& ElementalAttribute = EElementalAttribute::NEUTRAL)
{	
	if (bCanHurt == true)
	{
		FRotator RotAtAttacker;
		GetLookAtRotatorWithLocation(InstigatorLocation, RotAtAttacker);
		SetActorRotation(RotAtAttacker);

		bCanHurt = false;
		ResetHurt = 0.0f;

		if (!GetStats())
		{
			return;
		}
		
		GetStats()->GetPhysicalDamage(InstigatorATK, fMultiplier, ElementalAttribute);
		
		if (GetStats()->GetCurrentHP() <= 0)
		{
			FwdImpulse = -1500.0f;
			UpImpulse = 500.0f;

			bIsKO = true;

			SetActorTickEnabled(false);
		}

		FVector LaunchVector;

		GetActorForwardDistance(FwdImpulse, LaunchVector);
		LaunchVector.Z = UpImpulse;

		GetCharacterMovement()->StopMovementImmediately();
		LaunchCharacter(LaunchVector, true, true);

		FwdImpulse = UKismetMathLibrary::Abs(FwdImpulse);

		UpImpulse = UKismetMathLibrary::Abs(UpImpulse);

		const float MaxImpulse = UKismetMathLibrary::FMax(FwdImpulse, UpImpulse);
		OnGetDamagedEvent.Broadcast(MaxImpulse);
	}
}

void ATOASCharacter::IsSeen()
{
	if (SeenWidgetComponent == nullptr)
	{
		return;
	}

	if (GetStats()->GetCurrentHP() <= 0)
	{
		SeenWidgetComponent->SetVisibility(false);
		TargetWidgetComponent->SetVisibility(false);
		return;
	}

	if (SeenWidgetComponent->IsVisible() == true)
	{
		return;
	}

	SeenWidgetComponent->SetVisibility(true);
	TargetWidgetComponent->SetVisibility(false);
}

void ATOASCharacter::IsUnseen()
{
	if (SeenWidgetComponent == nullptr)
	{
		return;
	}

	SeenWidgetComponent->SetVisibility(false);
	TargetWidgetComponent->SetVisibility(false);
}

void ATOASCharacter::IsTargeted()
{
	if (TargetWidgetComponent == nullptr)
	{
		return;
	}

	if (TargetWidgetComponent->IsVisible() == true)
	{
		return;
	}

	SeenWidgetComponent->SetVisibility(false);
	TargetWidgetComponent->SetVisibility(true);
}

void ATOASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

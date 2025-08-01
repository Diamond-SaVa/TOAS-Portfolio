// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.


#include "C_EnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AC_EnemyCharacter::AC_EnemyCharacter()
{
	bIsEnemy = true;
}

void AC_EnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TraceForPlayer();
}

void AC_EnemyCharacter::TraceForPlayer()
{
	if (bPlayerWasFound == true)
	{
		return;
	}
	
	if (GetMesh()->DoesSocketExist(SightOrigin) == false)
	{
		return;
	}

	FVector HeadLocation = GetMesh()->GetSocketLocation(SightOrigin);

	FRotator HeadRotation = GetMesh()->GetSocketRotation(SightOrigin);

	FVector FwdLocation = UKismetMathLibrary::GetForwardVector(HeadRotation) * SightDistance;

	FHitResult Hit;
	
	bPlayerWasFound = UKismetSystemLibrary::SphereTraceSingleForObjects(this, HeadLocation + FwdLocation,
		HeadLocation + FwdLocation, SightRadius, SightTargetType, false,
		{}, EDrawDebugTrace::ForOneFrame, Hit, true);

	if (bPlayerWasFound == true)
	{
		OnPlayerWasFound.Broadcast();
	}
		
}

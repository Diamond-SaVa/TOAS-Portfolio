// Copyright Epic Games, Inc. All Rights Reserved.
// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#include "TOASGameMode.h"

#include "C_WB_MainMenu.h"
#include "C_WidgetNavigationSystem.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ATOASGameMode::ATOASGameMode()
{
	
}

void ATOASGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass == nullptr)
	{
		return;
	}
	
	if (UUserWidget* Instance = CreateWidget<UUserWidget>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		MainMenuWidgetClass, FName("MainMenu")))
	{
		MainManuWidgetObject = Cast<UC_WB_MainMenu>(Instance);

		MainManuWidgetObject->AddToPlayerScreen(99);
	}

	// Create an instance of your custom navigation config
	TSharedRef<FNavigationConfig> GameNavigationConfig = MakeShareable(new UC_WidgetNavigationSystem());

	// Set the SlateApplication to use your custom config
	FSlateApplication::Get().SetNavigationConfig(GameNavigationConfig);
}

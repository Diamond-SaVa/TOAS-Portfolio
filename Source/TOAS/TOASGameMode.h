// Copyright Epic Games, Inc. All Rights Reserved.
// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TOASGameMode.generated.h"

class UC_WB_MainMenu;
class AC_SaveManager;

UCLASS(minimalapi)
class ATOASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATOASGameMode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	UC_WB_MainMenu* MainManuWidgetObject;
	
protected:
	UFUNCTION()
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UI, meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
};




// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_WB_MainMenu.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class TOAS_API UC_WB_MainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UButton* GetNewGameButton() const { return NewGameButton; }

	UFUNCTION(BlueprintCallable)
	UButton* GetSettingsButton() const { return SettingsButton; }

	UFUNCTION(BlueprintCallable)
	UButton* GetExitButton() const { return ExitButton; }

	UFUNCTION(BlueprintCallable, Category=UI)
	void ConnectPlayerToWidget();

	UFUNCTION(BlueprintCallable, Category=UI)
	void ConnectPlayerToGame();

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UButton* NewGameButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UButton* SettingsButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UButton* ExitButton;
};

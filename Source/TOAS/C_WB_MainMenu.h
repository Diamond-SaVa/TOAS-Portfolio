// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_WB_MainMenu.generated.h"

enum class EPromptControl : uint8;
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
	UButton* GetExitButton() const { return ExitButton; }

	UFUNCTION(BlueprintCallable, Category=UI)
	void ConnectPlayerToWidget();

	UFUNCTION(BlueprintCallable, Category=UI)
	void ConnectPlayerToGame();

	UFUNCTION(BlueprintCallable, Category=UI)
	void UpdateControlPrompts();

	UFUNCTION(BlueprintCallable, Category=UI)
	void SendControlPromptsToGI(const EPromptControl SelectedPrompt);

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UButton* NewGameButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UButton* ExitButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UButton* PCButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UButton* XboxButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UButton* PSButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UButton* SwitchButton;
};

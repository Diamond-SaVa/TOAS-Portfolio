// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_WB_MainMenu.generated.h"

class UC_UW_SelectButton;
enum class EPromptControl : uint8;
/**
 * Class for the Main Menu of the Game on Startup.
 */
UCLASS()
class TOAS_API UC_WB_MainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<UC_UW_SelectButton*> AllButtons ()
	{
		return {
			NewGameButton, ExitButton, PCButton, XboxButton, PSButton,
			SwitchButton, CreditsButton, ReturnFromCreditsButton
		};
	};

	UFUNCTION(BlueprintCallable, Category=UI)
	void ConnectPlayerToWidget();

	UFUNCTION(BlueprintCallable, Category=UI)
	void ConnectPlayerToGame();

	UFUNCTION(BlueprintCallable, Category=UI)
	void UpdateControlPrompts();

	UFUNCTION(BlueprintCallable, Category=UI)
	void SendControlPromptsToGI(const EPromptControl SelectedPrompt);

	UFUNCTION(BlueprintCallable, Category=UI)
	void SetEnableButtonsAll(const bool IsEnabled);

	UFUNCTION(BlueprintCallable, Category=UI)
	void SetEnableButtonSingle(const UC_UW_SelectButton* ButtonToDisable, const bool IsEnabled);

	UFUNCTION()
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UC_UW_SelectButton* NewGameButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UC_UW_SelectButton* ExitButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UC_UW_SelectButton* PCButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UC_UW_SelectButton* XboxButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UC_UW_SelectButton* PSButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UC_UW_SelectButton* SwitchButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UC_UW_SelectButton* CreditsButton;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess=true, BindWidget))
	UC_UW_SelectButton* ReturnFromCreditsButton;
};
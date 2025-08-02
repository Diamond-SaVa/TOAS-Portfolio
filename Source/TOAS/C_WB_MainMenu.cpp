// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.


#include "C_WB_MainMenu.h"

#include "C_GI_GameManager.h"
#include "C_StructsAndEnums.h"
#include "Components/Button.h"

void UC_WB_MainMenu::ConnectPlayerToWidget()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly input;
		input.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		input.SetWidgetToFocus(NewGameButton->TakeWidget());
		PC->SetInputMode(input);
		PC->SetShowMouseCursor(true);
	}
}

void UC_WB_MainMenu::ConnectPlayerToGame()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameOnly input;
		PC->SetInputMode(input);
		PC->SetShowMouseCursor(false);
	}
}

void UC_WB_MainMenu::UpdateControlPrompts()
{
	// Stop code execution if no button exists
	if (PCButton == nullptr || XboxButton == nullptr || PSButton == nullptr || SwitchButton == nullptr)
	{
		return;
	}
	// Creates a prompt variable and defaults it to PC
	EPromptControl SelectedPrompt = EPromptControl::PC;
	// If casting to UC_GI_Manager from the GameInstance is successful,
	if (const UC_GI_GameManager* GI_GameManager = Cast<UC_GI_GameManager>(GetWorld()->GetGameInstance()))
	{
		// game will obtain the currently assigned prompt in the custom GameInstance.
		SelectedPrompt = GI_GameManager->GetPromptControl();
	}
	
	// Change the selected button's color to indicate that this is the current prompt, and the rest back to default.
	switch (SelectedPrompt)
	{
	case EPromptControl::PC:
		PCButton->SetColorAndOpacity(FLinearColor::Yellow);
		XboxButton->SetColorAndOpacity(FLinearColor::White);
		PSButton->SetColorAndOpacity(FLinearColor::White);
		SwitchButton->SetColorAndOpacity(FLinearColor::White);
		break;
	case EPromptControl::XBOX:
		PCButton->SetColorAndOpacity(FLinearColor::White);
		XboxButton->SetColorAndOpacity(FLinearColor::Yellow);
		PSButton->SetColorAndOpacity(FLinearColor::White);
		SwitchButton->SetColorAndOpacity(FLinearColor::White);
		break;
	case EPromptControl::PS:
		PCButton->SetColorAndOpacity(FLinearColor::White);
		XboxButton->SetColorAndOpacity(FLinearColor::White);
		PSButton->SetColorAndOpacity(FLinearColor::Yellow);
		SwitchButton->SetColorAndOpacity(FLinearColor::White);
		break;
	case EPromptControl::SWITCH:
		PCButton->SetColorAndOpacity(FLinearColor::White);
		XboxButton->SetColorAndOpacity(FLinearColor::White);
		PSButton->SetColorAndOpacity(FLinearColor::White);
		SwitchButton->SetColorAndOpacity(FLinearColor::Yellow);
		break;
	}
}

void UC_WB_MainMenu::SendControlPromptsToGI(const EPromptControl SelectedPrompt)
{
	if (UC_GI_GameManager* GI_GameManager = Cast<UC_GI_GameManager>(GetWorld()->GetGameInstance()))
	{
		GI_GameManager->SetPromptControl(SelectedPrompt);
	}
}

void UC_WB_MainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ConnectPlayerToGame();
	UpdateControlPrompts();
}

// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.


#include "C_WB_MainMenu.h"
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

void UC_WB_MainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ConnectPlayerToGame();
}

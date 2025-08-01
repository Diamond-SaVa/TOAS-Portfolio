// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.


#include "C_WB_SaveProcess.h"
#include "Components/Image.h"

void UC_WB_SaveProcess::ShowSavingIcon()
{
	SavingIcon->SetVisibility(ESlateVisibility::Hidden);
	LoadingIcon->SetVisibility(ESlateVisibility::Collapsed);
}

void UC_WB_SaveProcess::ShowLoadingIcon()
{
	SavingIcon->SetVisibility(ESlateVisibility::Collapsed);
	LoadingIcon->SetVisibility(ESlateVisibility::Hidden);
}

void UC_WB_SaveProcess::CompleteProcessAndRelease()
{
	SavingIcon->SetVisibility(ESlateVisibility::Hidden);
	LoadingIcon->SetVisibility(ESlateVisibility::Hidden);
	OnProcessIsDone.Broadcast();
}
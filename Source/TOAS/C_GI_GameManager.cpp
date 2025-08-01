// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.


#include "C_GI_GameManager.h"

void UC_GI_GameManager::UpdateChallengeOnSaveData(const FString& ChallengeID, const bool& bSuccess)
{
	// If there is no SaveGameObject instance being referenced to read from and write to,
	// do not proceed into the function.
	if (ChallengeID == "")
	{
		return;
	}

	// Otherwise, update the found Challenge's success boolean through the FString Key.
	SaveData.ChallengesList.Add(ChallengeID, bSuccess);
}

void UC_GI_GameManager::GetChallengeCompletionOnSaveData(const FString& ChallengeID, bool& bSuccess)
{
	// Start by defaulting the bSuccess reference to false.
	bSuccess = false;

	// If there is no SaveGameObject instance being referenced to read from,
	// do not proceed into the function.
	if (ChallengeID == "" || SaveData.ChallengesList.Contains(ChallengeID) == false)
	{
		return;
	}

	// Otherwise, check if the Challenge List contains the respective Challenge
	// to see it was already completed in this save file.
	bSuccess = SaveData.ChallengesList[ChallengeID];
}

void UC_GI_GameManager::UpdateCutsceneOnSaveData(const FString& CutsceneID, const bool& bWasPlayed)
{
	// If there is no SaveGameObject instance being referenced to read from and write to,
	// do not proceed into the function.
	if (CutsceneID == "")
	{
		return;
	}

	// Otherwise, update the found Cutscene that is done being played through the FString Key.
	SaveData.CutsceneList.Add(CutsceneID, bWasPlayed);
}

void UC_GI_GameManager::GetCutsceneCompletionOnSaveData(const FString& CutsceneID, bool& bWasPlayed)
{
	// Start by defaulting the bSuccess reference to false.
	bWasPlayed = false;

	// If there is no SaveGameObject instance being referenced to read from,
	// do not proceed into the function.
	if (CutsceneID == "" || SaveData.CutsceneList.Contains(CutsceneID) == false)
	{
		return;
	}

	// Otherwise, check if the Challenge List contains the respective Challenge
	// to see it was already completed in this save file.
	bWasPlayed = SaveData.CutsceneList[CutsceneID];
}

void UC_GI_GameManager::UpdateDialogueOnSaveData(const FString& DialogueID, const bool& bWasPlayed)
{
	// If there is no SaveGameObject instance being referenced to read from and write to,
	// do not proceed into the function.
	if (DialogueID == "")
	{
		return;
	}

	// Otherwise, update the found Cutscene that is done being played through the FString Key.
	SaveData.DialogueTriggersList.Add(DialogueID, bWasPlayed);
}

void UC_GI_GameManager::GetDialogueCompletionOnSaveData(const FString& DialogueID, bool& bWasPlayed)
{
	// Start by defaulting the bSuccess reference to false.
	bWasPlayed = false;

	// If there is no SaveGameObject instance being referenced to read from,
	// do not proceed into the function.
	if (DialogueID == "" || SaveData.DialogueTriggersList.Contains(DialogueID) == false)
	{
		return;
	}

	// Otherwise, check if the Challenge List contains the respective Challenge
	// to see it was already completed in this save file.
	bWasPlayed = SaveData.DialogueTriggersList[DialogueID];
}
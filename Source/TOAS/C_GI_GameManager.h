// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "C_StructsAndEnums.h"
#include "Engine/GameInstance.h"
#include "C_GI_GameManager.generated.h"

/**
 * 
 */

UCLASS()
class TOAS_API UC_GI_GameManager : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DATA")
	FSaveData SaveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DATA")
	EPromptControl PromptControl = EPromptControl::PC;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DATA_Getters")
	EPromptControl GetPromptControl() const { return PromptControl; };

	UFUNCTION(BlueprintCallable, Category = "DATA_Setters")
	void SetPromptControl(const EPromptControl& NewPrompt) { PromptControl = NewPrompt; };
	
	UFUNCTION(BlueprintCallable, Category = "SaveData_Functions")
	void UpdateChallengeOnSaveData(const FString& ChallengeID, const bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SaveData_Functions")
	void GetChallengeCompletionOnSaveData(const FString& ChallengeID, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SaveData_Functions")
	void UpdateCutsceneOnSaveData(const FString& CutsceneID, const bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SaveData_Functions")
	void GetCutsceneCompletionOnSaveData(const FString& CutsceneID, bool& bWasPlayed);

	UFUNCTION(BlueprintCallable, Category = "SaveData_Functions")
	void UpdateDialogueOnSaveData(const FString& DialogueID, const bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SaveData_Functions")
	void GetDialogueCompletionOnSaveData(const FString& DialogueID, bool& bSuccess);
};

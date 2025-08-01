// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "C_WB_SaveProcess.generated.h"

class UImage;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProcessDone);
/**
 * 
 */
UCLASS()
class TOAS_API UC_WB_SaveProcess : public UUserWidget
{
	GENERATED_BODY()

public:
	// Delegate for calling out to when Attacks are a Hit (usually in the enemy side). 
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnProcessDone OnProcessIsDone;
		
	UFUNCTION(BlueprintCallable, Category=UI)
	void ShowSavingIcon();

	UFUNCTION(BlueprintCallable, Category=UI)
	void ShowLoadingIcon();

	UFUNCTION(BlueprintCallable, Category=UI)
	void CompleteProcessAndRelease();

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* SavingIcon;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* LoadingIcon;
};

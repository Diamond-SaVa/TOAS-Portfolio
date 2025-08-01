// The original code and content of this project is dedicated to the showcase of my (Ricardo Sánchez Villegas)
// programming skills in Unreal Engine under the MIT Licence.
// While others may use the provided code and content for their own projects, proper credit is required and appreciated.

#pragma once

#include "CoreMinimal.h"
#include "C_StructsAndEnums.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "C_AN_AttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class TOAS_API UC_AN_AttackNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	FAttackProperties AttackProperties;
	
};

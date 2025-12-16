// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LyraLabGameplayAbility.h"
#include "LyraLabGameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class LYRALAB_API ULyraLabGameplayAbility_Jump : public ULyraLabGameplayAbility
{
	GENERATED_BODY()

public:
	ULyraLabGameplayAbility_Jump();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
protected:
	

	UFUNCTION(BlueprintCallable, Category = "LyraLab|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "LyraLab|Ability")
	void CharacterJumpStop();
};

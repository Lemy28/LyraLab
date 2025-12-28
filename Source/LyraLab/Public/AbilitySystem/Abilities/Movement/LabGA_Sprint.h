// Copyright Lemy. All Rights Reserved.

#pragma once

#include "AbilitySystem/Abilities/LabGameplayAbility.h"
#include "LabGA_Sprint.generated.h"

class UGameplayEffect;
class AAlsCharacter;

/**
 * `ULabGA_Sprint`
 *
 *	Gameplay ability for sprinting.
 */
UCLASS(Abstract)
class LYRALAB_API ULabGA_Sprint : public ULabGameplayAbility
{
	GENERATED_BODY()

public:

	ULabGA_Sprint(const FObjectInitializer& ObjectInitializer);

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};

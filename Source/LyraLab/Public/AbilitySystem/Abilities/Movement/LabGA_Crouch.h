// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LabGameplayAbility.h"
#include "LabGA_Crouch.generated.h"

/**
 * ULabGA_Crouch
 *
 *	Gameplay ability for character crouching.
 */
UCLASS()
class LYRALAB_API ULabGA_Crouch : public ULabGameplayAbility
{
	GENERATED_BODY()

public:
	ULabGA_Crouch(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};

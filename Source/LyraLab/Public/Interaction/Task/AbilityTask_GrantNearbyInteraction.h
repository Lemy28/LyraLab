// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_GrantNearbyInteraction.generated.h"

/**
 * 
 */
UCLASS()
class LYRALAB_API UAbilityTask_GrantNearbyInteraction : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;

	static UAbilityTask_GrantNearbyInteraction* GrantNearbyInteraction(UGameplayAbility* OwningAbility, float InteractionScanRate, float InteractionScanRange);

private:
	void QueryInteractables();
	virtual void OnDestroy(bool bInOwnerFinished) override;
	FTimerHandle QueryTimerHandle;
	float InteractionScanRate;
	float InteractionScanRange;
	
	TMap<FObjectKey, FGameplayAbilitySpecHandle> InteractionAbilityCache;
};

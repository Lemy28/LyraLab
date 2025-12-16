// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilityTask_WaitForInteractableTargets.h"
#include "Interaction/InteractionQuery.h"
#include "AbilityTask_WaitForInteractableTargets_SingleLineTrace.generated.h"

/**
 * 
 */
UCLASS()
class LYRALAB_API
	UAbilityTask_WaitForInteractableTargets_SingleLineTrace : public UAbilityTask_WaitForInteractableTargets
{
	GENERATED_BODY()

protected:
	// ~ Begin GameplayTask Interface
	virtual void Activate() override;
	// ~ End GameplayTask Interface

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DefaultToSelf = "OwningAbility", HidePin = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitForInteractableTargets_SingleLineTrace* WaitForInteractableTargets_SingleLineTrace(UGameplayAbility* OwningAbility, FCollisionProfileName TraceProfile, FInteractionQuery InteractionQuery, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange = 200, float InteractionScanRate = 0.100, bool bShowDebug = false);

	virtual void OnDestroy(bool bInOwnerFinished) override;
	void PerformTrace();
private:
	UPROPERTY()
	FInteractionQuery InteractionQuery;
	
	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo StartLocation;
	
	float InteractionScanRange = 200;
	float InteractionScanRate = 0.100;
	bool bShowDebug = false;
	
	FTimerHandle TimerHandle;
};

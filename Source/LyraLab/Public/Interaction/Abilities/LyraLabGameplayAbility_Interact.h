// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LyraLabGameplayAbility.h"
#include "LyraLabGameplayAbility_Interact.generated.h"

class UIndicatorDescriptor;
class IInteractableTarget;
struct FInteractionOption;
/**
 * 
 */
UCLASS()
class LYRALAB_API ULyraLabGameplayAbility_Interact : public ULyraLabGameplayAbility
{
	GENERATED_BODY()
public:
	ULyraLabGameplayAbility_Interact();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void UpdateInteractions(const TArray<FInteractionOption>& InteractableTargets);

	UFUNCTION(BlueprintCallable)
	void TriggerInteractions();
	
protected:

	TArray<FInteractionOption> CurrentOptions;
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;
	
	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRate = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRange = 500;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> DefaultInteractionWidgetClass;

};

// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LabGameplayAbility.generated.h"

class ALabPlayerController;
class ALabCharacter;
class ULabHeroComponent;
/**
 * ELyraAbilityActivationPolicy
 *
 *	Defines how an ability is meant to activate.
 */
UENUM(BlueprintType)
enum class ELyraAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};
/**
 * 
 */
UCLASS(Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class LYRALAB_API ULabGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class ULabAbilitySystemComponent;
public:

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	ULabAbilitySystemComponent* GetLyraAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	ALabPlayerController* GetLabPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	ALabCharacter* GetLabCharacterFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|Ability")
	ULabHeroComponent* GetHeroComponentFromActorInfo() const;
	
	ELyraAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;

protected:

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Ability Activation")
	ELyraAbilityActivationPolicy ActivationPolicy;
};

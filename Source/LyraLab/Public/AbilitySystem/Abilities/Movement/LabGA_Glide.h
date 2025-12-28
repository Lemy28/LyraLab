// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LabGameplayAbility.h"
#include "LabGA_Glide.generated.h"

/**
 * ULabGA_Fly
 *
 * Implements a "hold-to-glide" ability by reducing gravity while the input is active.
 * Can only be activated while falling and above a minimum height.
 * Automatically ends if the character gets too close to the ground.
 */
UCLASS()
class LYRALAB_API ULabGA_Glide : public ULabGameplayAbility
{
	GENERATED_BODY()

public:
	ULabGA_Glide(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGameplayAbility interface

	/** The callback for the periodic ground check task. */
	UFUNCTION()
	void OnGroundCheck();

	/** The gravity scale to apply when gliding. Configurable in Blueprint. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Glide")
	float GlidingGravityScale;

	/** The minimum height above ground to maintain gliding.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Glide")
	float MinGlidingHeight;

	/** The minimum height above ground to activate gliding.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Glide")
	float MinStartGlidingHeight;
	
	/** How often (in seconds) to check for ground distance while gliding. */	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Glide")
	float GroundCheckInterval;
	
	/** The Air Control to apply when gliding. Configurable in Blueprint. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Glide")
	float GlidingAirControl;

	/** The maximum downward speed (magnitude) allowed when gliding. Configurable in Blueprint. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Glide")
	float MaxGlidingFallSpeed;
	
	/** The maximum horizontal speed allowed when gliding. Configurable in Blueprint. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Glide")
	float MaxGlidingHorizontalSpeed;
	
	/** The braking deceleration to apply to horizontal movement when gliding and no input is provided. Configurable in Blueprint. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Glide")
	float GlidingBrakingDecelerationFalling;

private:
	// The character's original gravity scale, stored before we modify it.
	float OriginalGravityScale;
	
	// The character's original air control, stored before we modify it.
	float OriginalAirControl;

	// The character's original braking deceleration while falling, stored before we modify it.
	float OriginalBrakingDecelerationFalling;

	// bool bOriginalUseControllerDesiredRotation;
};
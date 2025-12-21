// Copyright Lemy. All Rights Reserved.

#include "AbilitySystem/Abilities/Movement/LabGA_Crouch.h"
#include "AbilitySystem/LabAbilitySystemComponent.h"
#include "ALS/Public/AlsCharacter.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/Abilities/LabGameplayAbility.h"
#include "Character/LabCharacter.h"
// #include "AlsGameplayTags.h"

ULabGA_Crouch::ULabGA_Crouch(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// For a toggle ability, it should be non-instanced and activate on input triggered.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	ActivationPolicy = ELyraAbilityActivationPolicy::OnInputTriggered;
}

void ULabGA_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AAlsCharacter* AlsCharacter = Cast<AAlsCharacter>(GetAvatarActorFromActorInfo());
	if (AlsCharacter)
	{
		// Toggle logic based on original Input_Crouch in ULabHeroComponent
		if (AlsCharacter->GetDesiredStance() == AlsStanceTags::Standing)
		{
			AlsCharacter->SetDesiredStance(AlsStanceTags::Crouching);
		}
		else if (AlsCharacter->GetDesiredStance() == AlsStanceTags::Crouching)
		{
			AlsCharacter->SetDesiredStance(AlsStanceTags::Standing);
		}
	}
	else
	{
		// Fallback for non-ALS character, try to toggle with generic Crouch/UnCrouch
		ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
		if (Character)
		{
			if (Character->bIsCrouched)
			{
				Character->UnCrouch();
			}
			else
			{
				Character->Crouch();
			}
		}
	}

	// For a non-instanced, toggle ability, it ends immediately after execution.
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

// EndAbility override is not strictly necessary for a non-instanced, OnInputTriggered ability that ends itself.
// However, to keep the virtual function intact, we can just call Super::EndAbility.
void ULabGA_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
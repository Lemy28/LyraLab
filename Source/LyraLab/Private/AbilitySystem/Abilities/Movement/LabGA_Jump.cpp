// Copyright Lemy. All Rights Reserved.

#include "AbilitySystem/Abilities/Movement/LabGA_Jump.h"
#include "AbilitySystem/LabAbilitySystemComponent.h"
#include "ALS/Public/AlsCharacter.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/Abilities/LabGameplayAbility.h"
#include "Character/LabCharacter.h"
// #include "AlsGameplayTags.h"

ULabGA_Jump::ULabGA_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// This ability should be instanced to handle the 'hold and release' input for variable jump height.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	// The activation policy should be 'WhileInputActive' to get the InputReleased callback.
	ActivationPolicy = ELyraAbilityActivationPolicy::WhileInputActive;
}

void ULabGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AAlsCharacter* AlsCharacter = Cast<AAlsCharacter>(GetAvatarActorFromActorInfo());
	if (!AlsCharacter)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// If the character is crouching, stand up instead of jumping.
	if (AlsCharacter->GetStance() == AlsStanceTags::Crouching)
	{
		AlsCharacter->SetDesiredStance(AlsStanceTags::Standing);
		// EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	// If not crouching, perform the jump.
	// The ability remains active until the input is released.
	AlsCharacter->Jump();
}

void ULabGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		Character->StopJumping();
	}
	
	// End the ability when the input is released.
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void ULabGA_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// It's important to call StopJumping here as well, in case the ability is cancelled by another action
	// before the input is released.
	if (bWasCancelled)
	{
		ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
		if (Character)
		{
			Character->StopJumping();
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

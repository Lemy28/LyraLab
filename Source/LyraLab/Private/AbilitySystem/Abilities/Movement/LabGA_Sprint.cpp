// Copyright Lemy. All Rights Reserved.

#include "AbilitySystem/Abilities/Movement/LabGA_Sprint.h"
#include "AlsCharacter.h"
#include "Utility/AlsGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ULabGA_Sprint::ULabGA_Sprint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationPolicy = ELyraAbilityActivationPolicy::WhileInputActive;
}

bool ULabGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const AAlsCharacter* Character = Cast<AAlsCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character || !Character->GetCharacterMovement())
	{
		return false;
	}

	if (Character->GetCharacterMovement()->IsFalling())
	{
		return false;
	}
	
	return Character->CanSprint();
}

void ULabGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AAlsCharacter* Character = Cast<AAlsCharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		Character->SetDesiredGait(AlsGaitTags::Sprinting, false);
	}
}

void ULabGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AAlsCharacter* Character = Cast<AAlsCharacter>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		Character->SetDesiredGait(AlsGaitTags::Running, false);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

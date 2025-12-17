// Copyright Lemy. All Rights Reserved.


#include "AbilitySystem/Abilities/LyraLabGameplayAbility_Jump.h"

#include "Character/LabCharacter.h"

ULyraLabGameplayAbility_Jump::ULyraLabGameplayAbility_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool ULyraLabGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}
	const ALabCharacter* LyraLabCharacter = Cast<ALabCharacter>(ActorInfo->AvatarActor.Get());
	if (!IsValid(LyraLabCharacter) || !LyraLabCharacter->CanJump())
	{
		return false;
	}
	
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void ULyraLabGameplayAbility_Jump::CharacterJumpStart()
{
	if (ALabCharacter* LyraCharacter = GetLabCharacterFromActorInfo())
	{
		if (LyraCharacter->IsLocallyControlled() && !LyraCharacter->bPressedJump)
		{
			LyraCharacter->UnCrouch();
			LyraCharacter->Jump();
		}
	}
}

void ULyraLabGameplayAbility_Jump::CharacterJumpStop()
{
	if (ALabCharacter* LyraCharacter = GetLabCharacterFromActorInfo())
	{
		if (LyraCharacter->IsLocallyControlled() && LyraCharacter->bPressedJump)
		{
			LyraCharacter->StopJumping();
		}
	}
}

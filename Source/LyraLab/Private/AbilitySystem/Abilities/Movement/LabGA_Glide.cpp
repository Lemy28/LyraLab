// Copyright Lemy. All Rights Reserved.

#include "AbilitySystem/Abilities/Movement/LabGA_Glide.h"

#include "AlsCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameFramework/Controller.h"

ULabGA_Glide::ULabGA_Glide(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationPolicy = ELyraAbilityActivationPolicy::WhileInputActive;
	
	OriginalGravityScale = 1.0f;
	OriginalAirControl = 0.0f;
	GlidingGravityScale = 0.1f;
	MinGlidingHeight = 100.0f;
	MinStartGlidingHeight = 200.0f;
	GroundCheckInterval = 0.05f;
	GlidingAirControl = 0.6f;
	MaxGlidingFallSpeed = 400.0f;
	MaxGlidingHorizontalSpeed = 800.0f;
}

bool ULabGA_Glide::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character || !Character->GetCharacterMovement() || !GetWorld())
	{
		return false;
	}

	if (!Character->GetCharacterMovement()->IsFalling())
	{
		return false;
	}

	const FVector StartLocation = Character->GetActorLocation();
	const FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, MinStartGlidingHeight + Character->GetDefaultHalfHeight());

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params))
	{
		if (HitResult.Distance < MinStartGlidingHeight)
		{
			return false;
		}
	}
	
	return true;
}

void ULabGA_Glide::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AAlsCharacter* Character = Cast<AAlsCharacter>(GetAvatarActorFromActorInfo());
	if (Character && Character->GetCharacterMovement())
	{
		UCharacterMovementComponent* CMC = Character->GetCharacterMovement();
		
		if (!CMC->IsFalling())
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}
		// Character->SetOverlayMode(AlsOverlayModeTags::Glide);
		
		FVector ClampedVelocity = CMC->Velocity;

		if (ClampedVelocity.Z > 0.0f)
		{
			ClampedVelocity.Z = 0.0f;
		}
		else if (ClampedVelocity.Z < -MaxGlidingFallSpeed)
		{
			ClampedVelocity.Z = -MaxGlidingFallSpeed;
		}
		CMC->Velocity = ClampedVelocity;
		OriginalGravityScale = CMC->GravityScale;
		OriginalAirControl = CMC->AirControl;
		OriginalBrakingDecelerationFalling = CMC->BrakingDecelerationFalling;
		
		CMC->MaxWalkSpeed = MaxGlidingHorizontalSpeed;
		CMC->AirControl = GlidingAirControl;
		CMC->BrakingDecelerationFalling = GlidingBrakingDecelerationFalling;

		OnGroundCheck();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void ULabGA_Glide::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AAlsCharacter* Character = Cast<AAlsCharacter>(GetAvatarActorFromActorInfo());
	if (Character && Character->GetCharacterMovement())
	{
		UCharacterMovementComponent* CMC = Character->GetCharacterMovement();
		CMC->GravityScale = OriginalGravityScale;
		CMC->AirControl = OriginalAirControl;
		CMC->BrakingDecelerationFalling = OriginalBrakingDecelerationFalling;

		if (!CMC->IsFalling())
		{
			Character->SetDesiredGait(AlsGaitTags::Running, false);
			// Character->SetOverlayMode(AlsOverlayModeTags::Default);
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULabGA_Glide::OnGroundCheck()
{
	if (!IsActive())
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character || !Character->GetCharacterMovement() || !GetWorld())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	UCharacterMovementComponent* CMC = Character->GetCharacterMovement();

	FVector CurrentVelocity = CMC->Velocity;
	FVector CurrentHorizontalVelocity = CurrentVelocity;
	CurrentHorizontalVelocity.Z = 0.0f; // Remove vertical component for horizontal speed check

	if (CurrentHorizontalVelocity.Size() > MaxGlidingHorizontalSpeed)
	{
		CurrentHorizontalVelocity = CurrentHorizontalVelocity.GetSafeNormal() * MaxGlidingHorizontalSpeed;
		CMC->Velocity = FVector(CurrentHorizontalVelocity.X, CurrentHorizontalVelocity.Y, CurrentVelocity.Z);
	}
	
	// Dynamically adjust gravity to maintain terminal velocity
	if (FMath::Abs(CMC->Velocity.Z) < MaxGlidingFallSpeed)
	{
		CMC->GravityScale = GlidingGravityScale;
	}
	else
	{
		CMC->GravityScale = 0.0f;
	}

	// Old line trace logic (commented out for optimization)
	// FHitResult HitResult;
	// const FVector StartLocation = Character->GetActorLocation();
	// const FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, MinGlidingHeight + Character->GetDefaultHalfHeight());
	// FCollisionQueryParams Params;
	// Params.AddIgnoredActor(Character);
	// if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params))
	// {
	// 	if (HitResult.Distance <= MinGlidingHeight)
	// 	{
	// 		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	// 		return;
	// 	}
	// }

	// New ground check using CharacterMovementComponent's cached floor data.
	// This optimizes the check by avoiding redundant physics queries.
	if (CMC->CurrentFloor.FloorDist <= MinGlidingHeight && CMC->CurrentFloor.IsWalkableFloor())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, GroundCheckInterval);
	if (DelayTask)
	{
		DelayTask->OnFinish.AddDynamic(this, &ULabGA_Glide::OnGroundCheck);
		DelayTask->ReadyForActivation();
	}
}
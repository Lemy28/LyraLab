// Copyright Lemy. All Rights Reserved.


#include "Interaction/Task/AbilityTask_GrantNearbyInteraction.h"
#include "Engine/OverlapResult.h"

#include "AbilitySystemComponent.h"
#include "Interaction/InteractableTarget.h"
#include "Interaction/InteractionQuery.h"
#include "Interaction/InteractionStatics.h"
#include "Physics/LyraLabCollisionChannel.h"


void UAbilityTask_GrantNearbyInteraction::Activate()
{
	Super::Activate();

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(QueryTimerHandle, this, &ThisClass::QueryInteractables, InteractionScanRate, true);
	}
}

UAbilityTask_GrantNearbyInteraction* UAbilityTask_GrantNearbyInteraction::GrantNearbyInteraction(
	UGameplayAbility* OwningAbility, float InteractionScanRate, float InteractionScanRange)
{
	UAbilityTask_GrantNearbyInteraction* MyTask = NewAbilityTask<UAbilityTask_GrantNearbyInteraction>(OwningAbility);
	MyTask->InteractionScanRange = InteractionScanRange;
	MyTask->InteractionScanRate = InteractionScanRate;
	return MyTask;
}

void UAbilityTask_GrantNearbyInteraction::QueryInteractables()
{
	UWorld* World = GetWorld();
	AActor* ActorOwner = GetAvatarActor();
	
	if (World && ActorOwner)
	{
		FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_GrantNearbyInteraction), false);

		TArray<FOverlapResult> OverlapResults;
		World->OverlapMultiByChannel(OUT OverlapResults, ActorOwner->GetActorLocation(), FQuat::Identity, LyraLab_TraceChannel_Interaction, FCollisionShape::MakeSphere(InteractionScanRange), Params);
// #ifdef ENABLE_DRAW_DEBUG
// 		DrawDebugSphere(World,
// 			ActorOwner->GetActorLocation(),
// 			InteractionScanRange,
// 			24,
// 			FColor::Yellow,
// 			false,
// 			InteractionScanRate);
// #endif

		
		if (OverlapResults.Num() > 0)
		{
			TArray<TScriptInterface<IInteractableTarget>> InteractableTargets;
			UInteractionStatics::AppendInteractableTargetsFromOverlapResults(OverlapResults, OUT InteractableTargets);
			
			FInteractionQuery InteractionQuery;
			InteractionQuery.RequestingAvatar = ActorOwner;
			InteractionQuery.RequestingController = Cast<AController>(ActorOwner->GetOwner());

			TArray<FInteractionOption> Options;
			for (TScriptInterface<IInteractableTarget>& InteractiveTarget : InteractableTargets)
			{
				FInteractionOptionBuilder InteractionBuilder(InteractiveTarget, Options);
				InteractiveTarget->GatherInteractionOptions(InteractionQuery, InteractionBuilder);
			}

			// Check if any of the options need to grant the ability to the user before they can be used.
			for (FInteractionOption& Option : Options)
			{
				if (Option.InteractionAbilityToGrant)
				{
					// Grant the ability to the GAS, otherwise it won't be able to do whatever the interaction is.
					FObjectKey ObjectKey(Option.InteractionAbilityToGrant);
					if (!InteractionAbilityCache.Find(ObjectKey))
					{
						FGameplayAbilitySpec Spec(Option.InteractionAbilityToGrant, 1, INDEX_NONE, this);
						if (Option.AbilityInputTag.IsValid())
						{
							Spec.GetDynamicSpecSourceTags().AddTag(Option.AbilityInputTag);
						}
						FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(Spec);
						InteractionAbilityCache.Add(ObjectKey, Handle);
					}
				}
			}
		}
	}
}

void UAbilityTask_GrantNearbyInteraction::OnDestroy(bool bInOwnerFinished)
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(QueryTimerHandle);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

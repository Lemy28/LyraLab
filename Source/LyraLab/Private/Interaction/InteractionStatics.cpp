// Copyright Lemy. All Rights Reserved.


#include "Interaction/InteractionStatics.h"
#include "Engine/OverlapResult.h"
#include "Interaction/InteractableTarget.h"

AActor* UInteractionStatics::GetActorFromInteractableTarget(TScriptInterface<IInteractableTarget> InteractableTarget)
{
	if (UObject* Object = InteractableTarget.GetObject())
	{
		if (AActor* Actor = Cast<AActor>(Object))
		{
			return Actor;
		}
		else if (UActorComponent* ActorComponent = Cast<UActorComponent>(Object))
		{
			return ActorComponent->GetOwner();
		}
		else
		{
			unimplemented();
		}
	}

	return nullptr;
}

void UInteractionStatics::AppendInteractableTargetsFromHitResult(FHitResult& HitResult,
                                                                 TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets)
{
	TScriptInterface<IInteractableTarget> InteractableTarget = HitResult.GetActor();
	if (InteractableTarget)
	{
		OutInteractableTargets.AddUnique(InteractableTarget);
	}
	TScriptInterface<IInteractableTarget> InteractableComponent = HitResult.GetComponent();
	if (InteractableComponent)
	{
		OutInteractableTargets.AddUnique(InteractableComponent);
	}
}

void UInteractionStatics::AppendInteractableTargetsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults,
	TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets)
{
	for (const FOverlapResult& Overlap : OverlapResults)
	{
		TScriptInterface<IInteractableTarget> InteractableTarget = Overlap.GetActor();
		if (InteractableTarget)
		{
			OutInteractableTargets.AddUnique(InteractableTarget);
		}
		TScriptInterface<IInteractableTarget> InteractableComponent = Overlap.GetComponent();
		if (InteractableComponent)
		{
			OutInteractableTargets.AddUnique(InteractableComponent);
		}
	}
}

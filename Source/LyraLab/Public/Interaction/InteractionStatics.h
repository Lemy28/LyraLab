// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionStatics.generated.h"

class IInteractableTarget;
/**
 * 
 */
UCLASS()
class LYRALAB_API UInteractionStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static AActor* GetActorFromInteractableTarget(TScriptInterface<IInteractableTarget> InteractableTarget);
	
	static void AppendInteractableTargetsFromHitResult(FHitResult& HitResult, TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets);
	static void AppendInteractableTargetsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets);
};

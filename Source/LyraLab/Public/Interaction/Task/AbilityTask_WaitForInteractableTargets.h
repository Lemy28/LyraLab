// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Interaction/InteractionOption.h"
#include "AbilityTask_WaitForInteractableTargets.generated.h"

struct FInteractionQuery;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableObjectsChangedEvent, const TArray<FInteractionOption>&,
                                            InteractableOptions);

/**
 * 
 */
UCLASS(Abstract)
class LYRALAB_API UAbilityTask_WaitForInteractableTargets : public UAbilityTask
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FInteractableObjectsChangedEvent InteractableObjectsChanged;

	static void LineTrace(FHitResult& OutHitResult, UWorld* World, FVector& Start, FVector& End, FName ProfileName, FCollisionQueryParams Params);
	static bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition) ;
	void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnorePitch = false) const;
	void UpdateInteractableOptions(const FInteractionQuery& InteractQuery, const TArray<TScriptInterface<IInteractableTarget>>& InteractableTargets);

protected:

	FCollisionProfileName TraceProfile;
	// bool bTraceAffectsAimPitch = true;
	TArray<FInteractionOption> CurrentOptions;
};

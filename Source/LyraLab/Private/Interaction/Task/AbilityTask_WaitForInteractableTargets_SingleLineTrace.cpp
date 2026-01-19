// Copyright Lemy. All Rights Reserved.


#include "Interaction/Task/AbilityTask_WaitForInteractableTargets_SingleLineTrace.h"

#include "Interaction/InteractionStatics.h"

void UAbilityTask_WaitForInteractableTargets_SingleLineTrace::Activate()
{
	Super::Activate();
	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformTrace, InteractionScanRate, true);
}

UAbilityTask_WaitForInteractableTargets_SingleLineTrace* UAbilityTask_WaitForInteractableTargets_SingleLineTrace::
WaitForInteractableTargets_SingleLineTrace(UGameplayAbility* OwningAbility, FCollisionProfileName TraceProfile,
	FInteractionQuery InteractionQuery, FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRange,
	float InteractionScanRate, bool bShowDebug)
{
	UAbilityTask_WaitForInteractableTargets_SingleLineTrace* MyObj = NewAbilityTask<UAbilityTask_WaitForInteractableTargets_SingleLineTrace>(OwningAbility);
	MyObj->InteractionScanRange = InteractionScanRange;
	MyObj->InteractionScanRate = InteractionScanRate;
	MyObj->StartLocation = StartLocation;
	MyObj->InteractionQuery = InteractionQuery;
	MyObj->TraceProfile = TraceProfile;
	MyObj->bShowDebug = bShowDebug;
	return MyObj;
}

void UAbilityTask_WaitForInteractableTargets_SingleLineTrace::OnDestroy(bool bInOwnerFinished)
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitForInteractableTargets_SingleLineTrace::PerformTrace()
{
	AActor* AvatarActor = Ability->GetAvatarActorFromActorInfo();
	if (AvatarActor == nullptr)
	{
		return;
	}

	UWorld* World = GetWorld();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);

	const bool bTraceComplex = false;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_WaitForInteractableTargets_SingleLineTrace), bTraceComplex);
	Params.AddIgnoredActors(ActorsToIgnore);

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
	FVector TraceEnd;
	AimWithPlayerController(AvatarActor, Params, TraceStart, InteractionScanRange, OUT TraceEnd);

	FHitResult OutHitResult;
	LineTrace(OutHitResult, World, TraceStart, TraceEnd, TraceProfile.Name, Params);

	TArray<TScriptInterface<IInteractableTarget>> InteractableTargets;
	UInteractionStatics::AppendInteractableTargetsFromHitResult(OutHitResult, InteractableTargets);

	UpdateInteractableOptions(InteractionQuery, InteractableTargets);

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DebugColor = OutHitResult.bBlockingHit ? FColor::Red : FColor::Green;
		if (OutHitResult.bBlockingHit)
		{
			DrawDebugLine(World, TraceStart, OutHitResult.Location, DebugColor, false, InteractionScanRate);
			DrawDebugSphere(World, OutHitResult.Location, 5, 16, DebugColor, false, InteractionScanRate);
		}
		else
		{
			DrawDebugLine(World, TraceStart, TraceEnd, DebugColor, false, InteractionScanRate);

			// --- 画胶囊体 (模拟 Sweep 的形状) ---
			int32 TraceRadius = 80;
			// 1. 计算中点：胶囊体需要画在 Start 和 End 的正中间
			FVector Center = (TraceStart + TraceEnd) * 0.5f;

			// 2. 计算长度和方向
			FVector TraceVector = TraceEnd - TraceStart;
			float TraceLength = TraceVector.Size();
    
			// 3. 计算旋转：DrawDebugCapsule 默认是竖着的 (Z轴)，我们需要把它转到 Trace 的方向
			// MakeFromZ 表示把 Z 轴对齐到我们的 Trace方向
			FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();

			// 4. 计算半高 (HalfHeight)：胶囊体的半高 = (圆柱体长度 / 2) + 半球半径
			float HalfHeight = (TraceLength * 0.5f) + TraceRadius;

			DrawDebugCapsule(
				World, 
				Center, 
				HalfHeight, 
				TraceRadius, 
				CapsuleRot, 
				FColor::Red, 
				false, 
				0.2
			);


		}
	}
#endif // ENABLE_DRAW_DEBUG
	

	
}


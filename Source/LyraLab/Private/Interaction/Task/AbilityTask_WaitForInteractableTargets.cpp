// Copyright Lemy. All Rights Reserved.


#include "Interaction/Task/AbilityTask_WaitForInteractableTargets.h"

#include "AbilitySystemComponent.h"
#include "Interaction/InteractableTarget.h"
#include "Interaction/InteractionQuery.h"

void UAbilityTask_WaitForInteractableTargets::LineTrace(FHitResult& OutHitResult, UWorld* World, FVector& Start, FVector& End, FName ProfileName, FCollisionQueryParams Params)
{
	check(World);

	OutHitResult = FHitResult();
	TArray<FHitResult> HitResults;
	
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);

	OutHitResult.TraceStart = Start;
	OutHitResult.TraceEnd = End;

	if (HitResults.Num() > 0)
	{
		OutHitResult = HitResults[0];
	}
	
}

/**
 * @brief 计算摄像机射线与一个以“技能中心”为球心、以“技能半径”为半径的球体的交点。
 *
 * @param CameraLocation   射线起点（摄像机的位置）
 * @param CameraDirection  射线的方向（摄像机的朝向，必须是单位向量）
 * @param AbilityCenter    球体的中心（通常是Pawn的位置）
 * @param AbilityRange     球体的半径（交互的最大范围）
 * @param ClippedPosition  [输出参数] 如果相交，这里会存储射线穿出球体的那个交点坐标
 * @return bool             如果射线成功与球体相交，则返回true
 */
bool UAbilityTask_WaitForInteractableTargets::ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition)
{
	// 1. 计算一个从"摄像机"指向"技能中心(Pawn)"的向量
	FVector CameraToCenter = AbilityCenter - CameraLocation;

	// 2. 将 CameraToCenter 向量 投影到 摄像机朝向(CameraDirection)上，得到投影长度
	// 这在数学上代表：摄像机需要沿着它的朝向走多远，才能到达与"技能中心"垂直距离最近的那个点
	float DotToCenter = FVector::DotProduct(CameraToCenter, CameraDirection);

	// 3. 检查投影长度
	// "技能中心"在摄像机的前面。
	if (DotToCenter >= 0)
	{
		// 4. 计算"摄像机视线(无限长)"与"技能中心"之间的最短垂直距离的【平方】
		float DistanceSquared = CameraToCenter.SizeSquared() - (DotToCenter * DotToCenter);

		// 5. 计算"技能范围(球体半径)"的【平方】
		float RadiusSquared = (AbilityRange * AbilityRange);

		// 6. 射线是否"错过"了球体？
		if (DistanceSquared <= RadiusSquared)
		{
			// 7. 射线确实穿过了球体，现在计算交点。
			float DistanceFromCamera = FMath::Sqrt(RadiusSquared - DistanceSquared);

			// 8. 计算从摄像机到"球体远端交点"的总距离
			// DistanceAlongRay = (摄像机到最近点的距离) + (最近点到球体边缘的距离)
			float DistanceAlongRay = DotToCenter + DistanceFromCamera;
			// (原始注释：如果用减法(DotToCenter - DistanceFromCamera)，会得到近端的交点)

			// 9. 计算出交点的世界坐标，并将其存入输出参数 ClippedPosition
			// 坐标 = 起点 + (方向 * 距离)
			ClippedPosition = CameraLocation + (DistanceAlongRay * CameraDirection);
			return true;
		}
	}
	return false;
}

void UAbilityTask_WaitForInteractableTargets::AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, float MaxRange, FVector& OutTraceEnd, bool bIgnorePitch) const
{
	if (!Ability) // Server and launching client only
	{
		return;
	}

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	check(PC);

	FVector ViewStart;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewStart, ViewRot);

	const FVector ViewDir = ViewRot.Vector();
	FVector ViewEnd = ViewStart + (ViewDir * MaxRange);

	ClipCameraRayToAbilityRange(ViewStart, ViewDir, TraceStart, MaxRange, ViewEnd);

	FHitResult HitResult;
	LineTrace(HitResult, InSourceActor->GetWorld(), ViewStart, ViewEnd, TraceProfile.Name, Params);

	const bool bUseTraceResult = HitResult.bBlockingHit && (FVector::DistSquared(TraceStart, HitResult.Location) <= (MaxRange * MaxRange));

	const FVector AdjustedEnd = (bUseTraceResult) ? HitResult.Location : ViewEnd;

	FVector AdjustedAimDir = (AdjustedEnd - TraceStart).GetSafeNormal();
	if (AdjustedAimDir.IsZero())
	{
		AdjustedAimDir = ViewDir;
	}

	// if (!bTraceAffectsAimPitch && bUseTraceResult)
	// {
	// 	FVector OriginalAimDir = (ViewEnd - TraceStart).GetSafeNormal();
	//
	// 	if (!OriginalAimDir.IsZero())
	// 	{
	// 		// Convert to angles and use original pitch
	// 		const FRotator OriginalAimRot = OriginalAimDir.Rotation();
	//
	// 		FRotator AdjustedAimRot = AdjustedAimDir.Rotation();
	// 		AdjustedAimRot.Pitch = OriginalAimRot.Pitch;
	//
	// 		AdjustedAimDir = AdjustedAimRot.Vector();
	// 	}
	// }

	OutTraceEnd = TraceStart + (AdjustedAimDir * MaxRange);
}
void UAbilityTask_WaitForInteractableTargets::UpdateInteractableOptions(const FInteractionQuery& InteractQuery, const TArray<TScriptInterface<IInteractableTarget>>& InteractableTargets)
{
	TArray<FInteractionOption> NewOptions;

	for (const TScriptInterface<IInteractableTarget>& InteractiveTarget : InteractableTargets)
	{
		TArray<FInteractionOption> TempOptions;
		FInteractionOptionBuilder InteractionBuilder(InteractiveTarget, TempOptions);
		InteractiveTarget->GatherInteractionOptions(InteractQuery, InteractionBuilder);

		for (FInteractionOption& Option : TempOptions)
		{
			FGameplayAbilitySpec* InteractionAbilitySpec = nullptr;

			// if there is a handle in a target ability system, we're triggering the ability on the target.
			if (Option.TargetAbilitySystem && Option.TargetInteractionAbilityHandle.IsValid())
			{
				// Find the spec
				InteractionAbilitySpec = Option.TargetAbilitySystem->FindAbilitySpecFromHandle(Option.TargetInteractionAbilityHandle);
			}
			// If there's an interaction ability then we're activating it on ourselves.
			else if (Option.InteractionAbilityToGrant)
			{
				// Find the spec
				InteractionAbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(Option.InteractionAbilityToGrant);

				if (InteractionAbilitySpec)
				{
					// update the option
					Option.TargetAbilitySystem = AbilitySystemComponent.Get();
					Option.TargetInteractionAbilityHandle = InteractionAbilitySpec->Handle;
				}
			}

			if (InteractionAbilitySpec)
			{
				// Filter any options that we can't activate right now for whatever reason.
				if (InteractionAbilitySpec->Ability->CanActivateAbility(InteractionAbilitySpec->Handle, AbilitySystemComponent->AbilityActorInfo.Get()))
				{
					NewOptions.Add(Option);
				}
			}
		}
	}

	bool bOptionsChanged = false;
	if (NewOptions.Num() == CurrentOptions.Num())
	{
		NewOptions.Sort();

		for (int OptionIndex = 0; OptionIndex < NewOptions.Num(); OptionIndex++)
		{
			const FInteractionOption& NewOption = NewOptions[OptionIndex];
			const FInteractionOption& CurrentOption = CurrentOptions[OptionIndex];

			if (NewOption != CurrentOption)
			{
				bOptionsChanged = true;
				break;
			}
		}
	}
	else
	{
		bOptionsChanged = true;
	}

	if (bOptionsChanged)
	{
		CurrentOptions = NewOptions;
		InteractableObjectsChanged.Broadcast(CurrentOptions);
	}
}

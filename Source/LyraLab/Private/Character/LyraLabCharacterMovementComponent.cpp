// Copyright Lemy. All Rights Reserved.


#include "Character/LyraLabCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

namespace LyraCharacter
{
	static float GroundTraceDistance = 100000.0f;
	FAutoConsoleVariableRef CVar_GroundTraceDistance(TEXT("LyraCharacter.GroundTraceDistance"), GroundTraceDistance, TEXT("Distance to trace down when generating ground information."), ECVF_Cheat);
};

void FSavedMove_LyraLab::Clear()
{
	FSavedMove_Character::Clear();
	bWantsToClimb = false;
}

void FSavedMove_LyraLab::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
	class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	if (const ULyraLabCharacterMovementComponent* MoveComp = Cast<ULyraLabCharacterMovementComponent>(C->GetCharacterMovement()))
	{
		bWantsToClimb = MoveComp->bWantsToClimb;
	}
}

bool FSavedMove_LyraLab::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	// return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
	if (!Super::CanCombineWith(NewMove, InCharacter, MaxDelta))
		return false;

	const FSavedMove_LyraLab* Other = static_cast<const FSavedMove_LyraLab*>(NewMove.Get());
	return bWantsToClimb == Other->bWantsToClimb;
}

void FSavedMove_LyraLab::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	if (ULyraLabCharacterMovementComponent* MoveComp = Cast<ULyraLabCharacterMovementComponent>(C->GetCharacterMovement()))
	{
		MoveComp->bWantsToClimb = bWantsToClimb;
	}
}

// Sets default values for this component's properties
ULyraLabCharacterMovementComponent::ULyraLabCharacterMovementComponent()
{
	// 告诉系统使用你的自定义容器
	SetNetworkMoveDataContainer(NetworkMoveDataContainer);
}

class FNetworkPredictionData_Client* ULyraLabCharacterMovementComponent::GetPredictionData_Client() const
{
	// 这是一个懒加载模式 (Lazy Initialization)
	if (ClientPredictionData == nullptr)
	{
		// 这里的 const_cast 是必须的，因为我们要初始化一个成员变量，但函数是 const 的
		ULyraLabCharacterMovementComponent* MutableThis = const_cast<ULyraLabCharacterMovementComponent*>(this);
		// --- 关键点：使用你刚刚定义的 FNetworkPredictionData_Client_LyraLab ---
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_LyraLab(*this);
        
	}

	return ClientPredictionData;
}

void ULyraLabCharacterMovementComponent::SimulateMovement(float DeltaTime)
{
	if (bHasReplicatedAcceleration)
	{
		// Preserve our replicated acceleration
		const FVector OriginalAcceleration = Acceleration;
		Super::SimulateMovement(DeltaTime);
		Acceleration = OriginalAcceleration;
	}
	else
	{
		Super::SimulateMovement(DeltaTime);
	}
}

void ULyraLabCharacterMovementComponent::SetReplicatedAcceleration(const FVector& InAcceleration)
{
	bHasReplicatedAcceleration = true;
	Acceleration = InAcceleration;
}

void ULyraLabCharacterMovementComponent::PerformMovement(float DeltaTime)
{
	Super::PerformMovement(DeltaTime);
}

const FLyraCharacterGroundInfo& ULyraLabCharacterMovementComponent::GetGroundInfo()
{
	if (!CharacterOwner || (GFrameCounter == CachedGroundInfo.LastUpdateFrame))
	{
		return CachedGroundInfo;
	}
	if (MovementMode == MOVE_Walking)
	{
		CachedGroundInfo.GroundHitResult = CurrentFloor.HitResult;
		CachedGroundInfo.GroundDistance = 0.0f;
	}
	else
	{
		const UCapsuleComponent* CapsuleComp = CharacterOwner->GetCapsuleComponent();
		check(CapsuleComp);

		const float CapsuleHalfHeight = CapsuleComp->GetUnscaledCapsuleHalfHeight();
		const ECollisionChannel CollisionChannel = (UpdatedComponent ? UpdatedComponent->GetCollisionObjectType() : ECC_Pawn);
		const FVector TraceStart(GetActorLocation());
		const FVector TraceEnd(TraceStart.X, TraceStart.Y, (TraceStart.Z - LyraCharacter::GroundTraceDistance - CapsuleHalfHeight));

		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(LyraCharacterMovementComponent_GetGroundInfo), false, CharacterOwner);
		FCollisionResponseParams ResponseParam;
		InitCollisionParams(QueryParams, ResponseParam);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, CollisionChannel, QueryParams, ResponseParam);

		CachedGroundInfo.GroundHitResult = HitResult;
		CachedGroundInfo.GroundDistance = LyraCharacter::GroundTraceDistance;

		if (MovementMode == MOVE_NavWalking)
		{
			CachedGroundInfo.GroundDistance = 0.0f;
		}
		else if (HitResult.bBlockingHit)
		{
			CachedGroundInfo.GroundDistance = FMath::Max((HitResult.Distance - CapsuleHalfHeight), 0.0f);
		}
	}

	CachedGroundInfo.LastUpdateFrame = GFrameCounter;

	return CachedGroundInfo;
}

void ULyraLabCharacterMovementComponent::StartClimbing()
{
	// 设置主模式为 Custom，子模式为 Climbing
	SetMovementMode(MOVE_Custom, CMOVE_Climbing);
}

void ULyraLabCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);
	// 根据子模式分发
	switch (CustomMovementMode)
	{
	case CMOVE_Climbing:
		PhysClimbing(deltaTime, Iterations);
		break;
	case CMOVE_Sliding:
		// PhysSliding(deltaTime, Iterations); // 另一个函数
		break;
	default:
		// 如果未知的自定义模式，直接退出或转为 Falling
		UE_LOG(LogTemp, Warning, TEXT("Invalid Custom Mode"));
		SetMovementMode(MOVE_Falling);
		break;
	}
}


void ULyraLabCharacterMovementComponent::PhysClimbing(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}
	// 计算输入移动（沿墙平面投影）
	// 处理吸附墙体
	// 按速度移动 safeMoveUpdatedComponent
	// SlideAlongSurface 保留贴墙
	// 检查是否失去攀爬条件

	
	// 1. 恢复上一帧的 Additive Velocity (处理 Root Motion Source 或外部力)
	// 这是 boilerplate 代码，几乎所有 PhysX 函数都要写
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		//RestorePreAdditiveVelocity();
	}

	// 2. 计算目标速度 (Velocity)
	// 假设：爬墙时，速度 = 输入向量 * 爬墙速度
	// 注意：GetInputVector() 是已经处理过的玩家输入
	FVector DesiredVelocity = GetLastInputVector() * MaxCustomMovementSpeed; 
    
	// (可选) 在这里添加重力或摩擦力逻辑
	// Velocity = DesiredVelocity; // 简单赋值
    
	// 使用 CalcVelocity 让他有加速/减速惯性
	CalcVelocity(deltaTime, 0.f, false, GetMaxBrakingDeceleration()); 

	// 3. 执行移动 (MoveUpdatedComponent)
	// 这是真正修改 Transform 的地方
	FVector Delta = Velocity * deltaTime;
	FHitResult Hit(1.f);
    
	// SafeMoveUpdatedComponent 会处理简单的碰撞并返回 Hit 信息
	SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);

	// 4. 处理碰撞 (Handle Collision)
	if (Hit.Time < 1.f)
	{
		// 如果撞到了东西 (比如爬到了天花板)，处理滑动
		HandleImpact(Hit, deltaTime, Delta);
		SlideAlongSurface(Delta, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	// 5. 检查退出条件
	// 比如：如果玩家体力耗尽，或者按了跳跃键，或者速度为0太久
	if (/* check exit condition */ false)
	{
		SetMovementMode(MOVE_Falling);
		StartNewPhysics(deltaTime, Iterations); // 立即切换到新物理
	}
}
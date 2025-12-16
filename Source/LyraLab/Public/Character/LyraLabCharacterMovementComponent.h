// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LyraLabCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ELyraLabCustomMovementMode
{
	CMOVE_None = 0,
	CMOVE_Climbing = 1 UMETA(DisplayName = "Climbing"), // 自定义模式 1：爬墙
	CMOVE_Sliding  = 2 UMETA(DisplayName = "Sliding"), // 自定义模式 2：滑铲
	CMOVE_MAX,
};

class FSavedMove_LyraLab : public FSavedMove_Character
{
public:
	typedef FSavedMove_Character Super;
	FSavedMove_LyraLab() : bWantsToClimb(false) {}
	// --- 在这里定义你要同步的自定义变量 ---
	uint8 bWantsToClimb : 1; // 比如：玩家是否按下了爬墙键

	// 1. 重置数据
	virtual void Clear() override;
    
	// 2. 核心：从 CMC 获取数据存入 SavedMove (Client -> SavedMove)
	virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
    
	// 3. 核心：将数据压缩成 flag 发送给服务器 (SavedMove -> Server) 新版不用这个
	// virtual uint8 GetCompressedFlags() const override;

	// 4. 判断两个 Move 能否合并（优化带宽）
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
	// 5. 核心：在回滚时，把数据还原回 CMC (SavedMove -> Client Resimulation)
	virtual void PrepMoveFor(ACharacter* C) override;
};

// 2. 新增：真正的网络数据载体（这是文档里强调的核心）
struct FLyraLabNetworkMoveData : public FCharacterNetworkMoveData
{
	uint8 bWantsToClimb : 1;

	FLyraLabNetworkMoveData()
		: bWantsToClimb(false)
	{}

	virtual void ClientFillNetworkMoveData(const FSavedMove_Character& ClientMove, ENetworkMoveType MoveType) override
	{
		const FSavedMove_LyraLab& LyraMove = static_cast<const FSavedMove_LyraLab&>(ClientMove);
		bWantsToClimb = LyraMove.bWantsToClimb;
	}

	virtual bool Serialize(UCharacterMovementComponent& CharacterMovement, FArchive& Ar, UPackageMap* PackageMap, ENetworkMoveType MoveType) override
	{
		//Ar << bWantsToClimb;
		return true;
	}
};
struct FLyraLabNetworkMoveDataContainer : public FCharacterNetworkMoveDataContainer
{
	FLyraLabNetworkMoveDataContainer()
	{
		// 这一步是关键，确保容器使用你的自定义 Data 类
		NewMoveData = &CustomDefaultMoveData;
	}

	// 实例化一个你的自定义 Data 对象作为默认值
	FLyraLabNetworkMoveData CustomDefaultMoveData;
};


// 自定义客户端预测数据类
class FNetworkPredictionData_Client_LyraLab : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;

	FNetworkPredictionData_Client_LyraLab(const UCharacterMovementComponent& ClientMovement)
		: Super(ClientMovement)
	{}

	// 分配我们自己的 SavedMove
	virtual FSavedMovePtr AllocateNewMove() override
	{
		// 核心：这里实例化你的自定义 SavedMove 类
		//return FSavedMovePtr(new FSavedMove_LyraLab());
		return MakeShared<FSavedMove_LyraLab>();
	}
};


/**
 * FLyraCharacterGroundInfo
 *
 *	Information about the ground under the character.  It only gets updated as needed.
 */
USTRUCT(BlueprintType)
struct FLyraCharacterGroundInfo
{
	GENERATED_BODY()

	FLyraCharacterGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LYRALAB_API ULyraLabCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	ULyraLabCharacterMovementComponent();

	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	virtual void SimulateMovement(float DeltaTime) override;
	void SetReplicatedAcceleration(const FVector& InAcceleration);

	virtual void PerformMovement(float DeltaTime) override;
	const FLyraCharacterGroundInfo& GetGroundInfo();
protected:

	// Cached ground info for the character.  Do not access this directly!  It's only updated when accessed via GetGroundInfo().
	FLyraCharacterGroundInfo CachedGroundInfo;
	
	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration = false;

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartClimbing();

	bool bWantsToClimb = false;
protected:
	void PhysCustom(float deltaTime, int32 Iterations) override;
	void PhysClimbing(float deltaTime, int32 Iterations);

private:
	FLyraLabNetworkMoveDataContainer NetworkMoveDataContainer;
};

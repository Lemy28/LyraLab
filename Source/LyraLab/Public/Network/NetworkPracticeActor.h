// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetworkPracticeActor.generated.h"

UCLASS()
class LYRALAB_API ANetworkPracticeActor : public AActor
{
	GENERATED_BODY()
public:
	ANetworkPracticeActor();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// [Chapter 3] C++ 必须重写这个函数来配置属性复制规则
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// [Chapter 3] 一个只发给 Owner 的秘密变量 (蓝图做不到这种条件控制)
	UPROPERTY(ReplicatedUsing = OnRep_SecretCode)
	int32 SecretCode;

	// [Chapter 3] 标准的 RepNotify，带参数 (蓝图做不到带参数)
	UFUNCTION()
	void OnRep_SecretCode(int32 OldValue);

	// 一个计时器，用来测试同步
	float UpdateTimer;
};

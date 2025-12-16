#include "Network/NetworkPracticeActor.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ANetworkPracticeActor::ANetworkPracticeActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // [Chapter 1] 必须开启复制
    bReplicates = true;

    // [Chapter 1] 调整优先级和频率
    // 我们把它设得比 Pawn 低，但比普通背景物体高
    NetPriority = 2.0f;
    NetUpdateFrequency = 10.0f; // 每秒只同步10次，节省 CPU

    SecretCode = 999;
    UpdateTimer = 0.0f;

}

// [Chapter 3] C++ 独占的条件复制配置
void ANetworkPracticeActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 重点！COND_OwnerOnly 表示这个变量只会发给 Owner。
    // 旁观者（Simulated Proxy）虽然能看到这个 Actor，但收不到这个变量的更新！
    // DOREPLIFETIME_CONDITION(ANetworkPracticeActor, SecretCode, COND_None);

    // 修改后：发给所有人
    DOREPLIFETIME(ANetworkPracticeActor, SecretCode);
}

void ANetworkPracticeActor::BeginPlay()
{
    Super::BeginPlay();

    // [Chapter 5] 区分执行路径：我是什么模式？
    FString ModeString = "Unknown";
    switch (GetNetMode())
    {
        case NM_Standalone: ModeString = "Standalone (Single Player)"; break;
        case NM_DedicatedServer: ModeString = "Dedicated Server"; break;
        case NM_ListenServer: ModeString = "Listen Server (Host)"; break;
        case NM_Client: ModeString = "Client"; break;
    }

    // [Chapter 5] 区分执行路径：我有权限吗？
    FString AuthString = HasAuthority() ? "Authority (Boss)" : "Proxy (Viewer)";

    // 打印调试信息 (在屏幕左上角)
    // 注意：每个端都会打印自己看到的那一份
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, 
        FString::Printf(TEXT("[%s] I am running as: %s"), *AuthString, *ModeString));
}

void ANetworkPracticeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 只有服务器负责修改逻辑
    if (HasAuthority())
    {
        UpdateTimer += DeltaTime;
        if (UpdateTimer > 2.0f)
        {
            UpdateTimer = 0.0f;
            // 修改变量，触发同步
            SecretCode = FMath::RandRange(1000, 9999);
            
            // [Chapter 2] 安全的获取 PlayerController 演示
            // 假设我们想把这个 Actor 归属给第一个玩家 (仅作测试)
            // 这里我们必须小心，不能乱用 GetPlayerController(0)
            // 但为了演示 SetOwner 后的效果，我们手动指定一下
            if (!GetOwner())
            {
                // 这里的 GetFirstLocal... 在服务器上可能获取不到特定的远程玩家
                // 实际项目中，你应该在 Spawn 时传入 Owner
                // 这里仅作演示：如果没有 Owner，就还是公共的
            }
        }
    }
}

// [Chapter 3] 回调函数
void ANetworkPracticeActor::OnRep_SecretCode(int32 OldValue)
{
    // 客户端逻辑
    // 只有当 SecretCode 真的从服务器传过来，且值发生变化时才触发
    
    FString Msg = FString::Printf(TEXT("Client Event: SecretCode changed from %d to %d"), OldValue, SecretCode);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
    
    // 观察点：如果你不是 Owner，你将永远看不到这条 Log，即使 Actor 就在你眼前！
    // 因为我们在 GetLifetimeReplicatedProps 里用了 COND_OwnerOnly
}
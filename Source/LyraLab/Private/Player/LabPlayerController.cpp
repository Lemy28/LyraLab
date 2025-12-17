// Copyright Lemy. All Rights Reserved.


#include "Player/LabPlayerController.h"

#include "AbilitySystem/LyraLabAbilitySystemComponent.h"
#include "Character/LabCharacter.h"
#include "Character/LabPawnExtensionComponent.h"
#include "Player/LabPlayerState.h"

ULabAbilitySystemComponent* ALabPlayerController::GetLabAbilitySystemComponent() const
{
	const ALabPlayerState* PS = GetLabPlayerState();
	return (PS ? PS->GetLabAbilitySystemComponent() : nullptr);
}

ALabPlayerState* ALabPlayerController::GetLabPlayerState() const
{
	return CastChecked<ALabPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

void ALabPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);

}

void ALabPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (ULabAbilitySystemComponent* LyraASC = GetLabAbilitySystemComponent())
	{
		LyraASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	
	Super::PostProcessInput(DeltaTime, bGamePaused);
	
	// UE_LOG(LogTemp, Log, TEXT("RotationInput.Pitch = %f"), RotationInput.Pitch)
	// // 1. 格式化字符串
	// FString DebugMessage = FString::Printf(TEXT("RotationInput.Pitch = %f"), RotationInput.Pitch);
	//
	// // 2. 调用 AddOnScreenDebugMessage
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(
	// 		1,                       // Key: -1 表示每次都显示新消息，不替换旧的
	// 		0.1f,                     // TimeToDisplay: 持续时间（秒）
	// 		FColor::Yellow,// DisplayColor: 文本颜色
	// 		DebugMessage              // DebugMessage: 待显示的字符串
	// 	);
	// }
	
}

void ALabPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);
	ULabPawnExtensionComponent* PawnExtensionComponent = ULabPawnExtensionComponent::FindPawnExtensionComponent(P);
	ALabPlayerState* PS = Cast<ALabPlayerState>(PlayerState);
	if (PS && PawnExtensionComponent)
	{
		PawnExtensionComponent->InitializeAbilitySystem(PS->GetLabAbilitySystemComponent(), P);
	}
}

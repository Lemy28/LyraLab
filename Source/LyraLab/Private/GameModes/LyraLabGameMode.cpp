// Copyright Lemy. All Rights Reserved.


#include "GameModes/LyraLabGameMode.h"
#include "Character/LLPawnData.h"
#include "Player/LabPlayerState.h"

const ULyraLabPawnData* ALyraLabGameMode::GetPawnDataForController(const AController* InController) const
{
	// See if pawn data is already set on the player state
	if (InController != nullptr)
	{
		if (const ALabPlayerState* LyraPS = InController->GetPlayerState<ALabPlayerState>())
		{
			if (const ULyraLabPawnData* PawnData = LyraPS->GetPawnData<ULyraLabPawnData>())
			{
				return PawnData;
			}
		}
	}
	check(DefaultPawnData)
	return DefaultPawnData;
	// If not, fall back to the the default for the current experience
	// check(GameState);
	// ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	// check(ExperienceComponent);
	//
	// if (ExperienceComponent->IsExperienceLoaded())
	// {
	// 	const ULyraExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
	// 	if (Experience->DefaultPawnData != nullptr)
	// 	{
	// 		return Experience->DefaultPawnData;
	// 	}
	//
	// 	// Experience is loaded and there's still no pawn data, fall back to the default for now
	// 	return ULyraAssetManager::Get().GetDefaultPawnData();
	// }

	// Experience not loaded yet, so there is no pawn data to be had
	//return nullptr;
}

void ALyraLabGameMode::LogAllPlayerPings()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UNetDriver* NetDriver = World->GetNetDriver();
		if (NetDriver)
		{
			for (const auto& ClientConnection : NetDriver->ClientConnections)
			{
				float Latency = ClientConnection->AvgLag;
				// 假如该连接已经拥有了一个 PlayerController，我们可以打印出玩家的名字以便区分
				FString PlayerName = TEXT("Unknown");
				if (ClientConnection->PlayerController && ClientConnection->PlayerController->PlayerState)
				{
					PlayerName = ClientConnection->PlayerController->PlayerState->GetPlayerName();
				}

				UE_LOG(LogTemp, Log, TEXT("Player [%s] Ping: %f ms"), *PlayerName, Latency * 1000.0f);
			}
		}
	}
}

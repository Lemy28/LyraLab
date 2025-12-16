// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LyraLabGameMode.generated.h"


class ULyraLabPawnData;
/**
 * 
 */
UCLASS()
class LYRALAB_API ALyraLabGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const ULyraLabPawnData* GetPawnDataForController(const AController* InController) const;

	UFUNCTION(BlueprintCallable, Category = "NetworkDebugging")
	void LogAllPlayerPings();
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULyraLabPawnData> DefaultPawnData;
};

// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LabEquipmentInstance.generated.h"

struct FLabEquipmentActorToSpawn;
/**
 * 
 */
UCLASS()
class LYRALAB_API ULabEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	ULabEquipmentInstance();
	void SpawnEquipmentActors(const TArray<FLabEquipmentActorToSpawn>& EquipmentActors);


	APawn* GetPawn();
private:
	UPROPERTY()
	TArray<AActor*> SpawnedActors;
};

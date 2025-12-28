// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LabEquipmentDefinition.generated.h"

class ULabGameplayAbility;

USTRUCT()
struct FLabEquippmentActorToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<AActor> ActorToSpawn;
	UPROPERTY(EditAnywhere, Category = "Equipment")
	FTransform AttachTransform;
	UPROPERTY(EditAnywhere, Category = "Equipment")
	FName AttachSocket;
};


/**
 * 
 */
UCLASS()
class LYRALAB_API ULabEquipmentDefinition : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TArray<FLabEquippmentActorToSpawn> EquipmentActors;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TArray<TSubclassOf<ULabGameplayAbility>> AbilitiesToGrant;

	UPROPERTY()
	TArray<AActor*> SpawnedActors;
};

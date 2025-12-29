// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LabEquipmentDefinition.generated.h"

class ULabEquipmentInstance;
class ULabAbilitySet;
class ULabGameplayAbility;

USTRUCT()
struct FLabEquipmentActorToSpawn
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
UCLASS(Blueprintable, Abstract, Const)
class LYRALAB_API ULabEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TSubclassOf<ULabEquipmentInstance> InstanceType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<TObjectPtr<ULabAbilitySet>> AbilitySetsToGrant;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<FLabEquipmentActorToSpawn> EquipmentActors;

};

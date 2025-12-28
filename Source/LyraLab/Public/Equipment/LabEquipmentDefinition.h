// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LabEquipmentDefinition.generated.h"

class ULabEquipmentInstance;
class ULabAbilitySet;
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
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSubclassOf<ULabEquipmentInstance> LabEquipmentInstance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TArray<TObjectPtr<ULabAbilitySet>> AbilitySetsToGrant;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TArray<FLabEquippmentActorToSpawn> EquipmentActors;

};

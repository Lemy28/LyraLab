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
	void DestroyEquipmentActors();

	virtual void OnEquipped();
	virtual void OnUnequipped();
	
	APawn* GetPawn();
protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();
	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();
	
private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};

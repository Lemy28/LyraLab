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

	UFUNCTION(BlueprintPure, Category=Equipment)
	UObject* GetInstigator() const { return Instigator; }
	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End of UObject interface

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;
	
	void SpawnEquipmentActors(const TArray<FLabEquipmentActorToSpawn>& EquipmentActors);
	void DestroyEquipmentActors();
	virtual void OnEquipped();
	virtual void OnUnequipped();

private:
	UPROPERTY(ReplicatedUsing=OnRep_Instigator)
	TObjectPtr<UObject> Instigator;
protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();
	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();
private:
	UFUNCTION()
	void OnRep_Instigator();

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};

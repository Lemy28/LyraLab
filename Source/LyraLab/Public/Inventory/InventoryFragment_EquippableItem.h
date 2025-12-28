// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LabInventoryItemDefinition.h"
#include "InventoryFragment_EquippableItem.generated.h"

class ULabEquipmentDefinition;
/**
 * 
 */
UCLASS()
class LYRALAB_API UInventoryFragment_EquippableItem : public ULabInventoryFragment
{
	GENERATED_BODY()
	//Handled by EquipmentComponent
	// virtual void OnInstanceCreated(ULabInventoryItemInstance* NewInstance) const override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equippment")
	TSubclassOf<ULabEquipmentDefinition> ItemDefinition;
};

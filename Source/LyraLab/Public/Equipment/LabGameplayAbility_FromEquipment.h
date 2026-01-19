// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/LabGameplayAbility.h"
#include "LabGameplayAbility_FromEquipment.generated.h"

class ULabEquipmentInstance;
class ULabInventoryItemInstance;
/**
 * 
 */
UCLASS()
class LYRALAB_API ULabGameplayAbility_FromEquipment : public ULabGameplayAbility
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="Lab|Ability")
	ULabEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "Lab|Ability")
	ULabInventoryItemInstance* GetAssociatedItem() const;
};

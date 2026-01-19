// Copyright Lemy. All Rights Reserved.


#include "Equipment/LabGameplayAbility_FromEquipment.h"

#include "Equipment/LabEquipmentInstance.h"
#include "Inventory/LabInventoryItemInstance.h"

ULabEquipmentInstance* ULabGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<ULabEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

ULabInventoryItemInstance* ULabGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (ULabEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<ULabInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}

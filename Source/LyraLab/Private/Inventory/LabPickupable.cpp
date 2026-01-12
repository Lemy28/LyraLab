// Copyright Lemy. All Rights Reserved.


#include "Inventory/LabPickupable.h"


TScriptInterface<ILabPickupable> ULabInventoryBlueprintLibrary::GetPickupableFromActor(AActor* Actor)
{
	//TODO Support Pickupable Component
	return TScriptInterface<ILabPickupable>(Actor);
}

// Add default functionality here for any IPickupable functions that are not pure virtual.
void ULabInventoryBlueprintLibrary::AddPickupableToInventory(ULabInventoryManagerComponent* InventoryManagerComponent,
	TScriptInterface<ILabPickupable> LabPickupable)
{
	if (InventoryManagerComponent && LabPickupable)
	{
		const FInventoryPickup& InventoryPickup = LabPickupable->GetPickup();
		InventoryManagerComponent->add
	}
}

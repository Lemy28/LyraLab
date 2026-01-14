// Copyright Lemy. All Rights Reserved.


#include "Inventory/LabPickupable.h"

#include "Inventory/LabInventoryManagerComponent.h"
#include "Runtime/Engine/Internal/VT/VirtualTextureVisualizationData.h"


TScriptInterface<ILabPickupable> ULabPickupableStatics::GetPickupableFromActor(AActor* Actor)
{
	//TODO Support Pickupable Component
	return TScriptInterface<ILabPickupable>(Actor);
}

// Add default functionality here for any IPickupable functions that are not pure virtual.
void ULabPickupableStatics::AddPickupableToInventory(ULabInventoryManagerComponent* InventoryManagerComponent,
	TScriptInterface<ILabPickupable> LabPickupable)
{
	if (InventoryManagerComponent && LabPickupable)
	{
		const FInventoryPickup& InventoryPickup = LabPickupable->GetPickup();
		for (const auto& Instance : InventoryPickup.Instances)
		{
			//TODO 什么情况需要添加Instance？
		}
		for (const auto& Definition : InventoryPickup.Definitions)
		{
			InventoryManagerComponent->AddItemDefinition(Definition);
		}
	}
}

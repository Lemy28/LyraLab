// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inventory/LabInventoryManagerComponent.h"

#include "Inventory/LabInventoryItemDefinition.h"
#include "Inventory/LabInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"

void FLabInventoryList::AddEntry(TSubclassOf<ULabInventoryItemDefinition> ItemDefinition, int32 StackCount)
{
	FLabInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();

	NewEntry.Instance = NewObject<ULabInventoryItemInstance>(OwnerComponent);
	NewEntry.StackCount = StackCount;

	Entries.Add(NewEntry);
	
	MarkItemDirty(NewEntry);
	MarkArrayDirty();
}

void FLabInventoryList::AddEntry(ULabInventoryItemInstance* InventoryItemInstance)
{
	unimplemented();
}

void FLabInventoryList::RemoveEntry(ULabInventoryItemInstance* InventoryItemInstance)
{

	for (auto It = Entries.CreateIterator(); It; ++It)
	{
		if (It->Instance == InventoryItemInstance)
		{
			It.RemoveCurrent();
			MarkArrayDirty();
			return;
		}
	}
	
}

ULabInventoryManagerComponent::ULabInventoryManagerComponent()
{
}

void ULabInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

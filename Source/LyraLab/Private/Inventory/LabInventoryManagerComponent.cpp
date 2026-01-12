// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inventory/LabInventoryManagerComponent.h"

#include "Inventory/LabInventoryItemDefinition.h"
#include "Inventory/LabInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"

ULabInventoryItemInstance* FLabInventoryList::AddEntry(TSubclassOf<ULabInventoryItemDefinition> ItemDefinition, int32 StackCount)
{
	check(ItemDefinition);
	check(OwnerComponent);
	check(OwnerComponent->GetOwnerRole() == ROLE_Authority);

	
	FLabInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();

	NewEntry.Instance = NewObject<ULabInventoryItemInstance>(OwnerComponent->GetOwner());
	NewEntry.StackCount = StackCount;
	NewEntry.Instance->SetItemDef(ItemDefinition);
	
	auto ItemCDO = ItemDefinition->GetDefaultObject<ULabInventoryItemDefinition>();
	if (ItemCDO)
	{
		for ( const auto& Fragment : ItemCDO->Fragments)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	MarkItemDirty(NewEntry);
	return NewEntry.Instance;
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
	: InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

ULabInventoryItemInstance* ULabInventoryManagerComponent::AddItemDefinition(
	TSubclassOf<ULabInventoryItemDefinition> ItemDef, int32 StackCount)
{
	ULabInventoryItemInstance* Result = nullptr;

	if (ItemDef && StackCount > 0)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	
	return Result;
}

void ULabInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}



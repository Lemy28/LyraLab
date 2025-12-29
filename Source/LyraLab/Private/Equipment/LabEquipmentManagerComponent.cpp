// Copyright Lemy. All Rights Reserved.


#include "Equipment/LabEquipmentManagerComponent.h"

#include "Equipment/LabEquipmentDefinition.h"
#include "Equipment/LabEquipmentInstance.h"
#include "Net/UnrealNetwork.h"

FString FLabEquipmentEntry::GetDebugString()
{
	return "";
}

ULabEquipmentInstance* FLabEquipmentList::AddEntry(TSubclassOf<ULabEquipmentDefinition> EquipmentDefinition)
{
	check(OwnerComponent != nullptr);
	check(EquipmentDefinition)

	ULabEquipmentInstance* NewInstance = nullptr;
	
	FLabEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	auto DefaultObject = GetDefault<ULabEquipmentDefinition>(EquipmentDefinition);
	// auto DefaultObject =  EquipmentDefinition->StaticClass()->GetDefaultObject<ULabEquipmentDefinition>();
	NewInstance = NewObject<ULabEquipmentInstance>(OwnerComponent, DefaultObject->InstanceType);

	NewEntry.Definition = EquipmentDefinition;
	NewEntry.Instance = NewInstance;
	//TODO: Grant Abilities
	for (const auto& AbilitySetToGrant : DefaultObject->AbilitySetsToGrant)
	{
		
	}
	//Let the Instance Create Actors 
	
	NewInstance->SpawnEquipmentActors(&DefaultObject->EquipmentActors);
	MarkItemDirty(NewEntry);
	return NewInstance;	
}

ULabEquipmentManagerComponent::ULabEquipmentManagerComponent()
	: EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
}

void ULabEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

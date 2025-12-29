// Copyright Lemy. All Rights Reserved.


#include "Equipment/LabEquipmentManagerComponent.h"

#include "Equipment/LabEquipmentDefinition.h"
#include "Equipment/LabEquipmentInstance.h"
#include "Net/UnrealNetwork.h"

FString FLabEquipmentEntry::GetDebugString()
{
	return "";
}

void FLabEquipmentList::AddEntry(TSubclassOf<ULabEquipmentDefinition> EquipmentDefinition)
{
	check(OwningComponent != nullptr);
	check(EquipmentDefinition)

	ULabEquipmentInstance* NewInstance = nullptr;
	
	FLabEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	auto DefaultObject = GetDefault<ULabEquipmentDefinition>(EquipmentDefinition);
	// auto DefaultObject =  EquipmentDefinition->StaticClass()->GetDefaultObject<ULabEquipmentDefinition>();
	NewInstance = NewObject<ULabEquipmentInstance>(OwningComponent, DefaultObject->InstanceType);

	NewEntry.Definition = EquipmentDefinition;
	NewEntry.Instance = NewInstance;
	//TODO: Grant Abilities
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

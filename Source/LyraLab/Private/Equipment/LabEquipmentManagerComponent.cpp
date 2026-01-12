// Copyright Lemy. All Rights Reserved.


#include "Equipment/LabEquipmentManagerComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/LabAbilitySystemComponent.h"
#include "Equipment/LabEquipmentDefinition.h"
#include "Equipment/LabEquipmentInstance.h"
#include "Net/UnrealNetwork.h"

FString FLabEquipmentEntry::GetDebugString()
{
	return "";
}

ULabAbilitySystemComponent* FLabEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent)
	AActor* OwnerActor = OwnerComponent->GetOwner();
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);
	return Cast<ULabAbilitySystemComponent>(ASC);
}

void FLabEquipmentList::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const auto& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FLabEquipmentList::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const auto& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
	}
}

ULabEquipmentInstance* FLabEquipmentList::AddEntry(TSubclassOf<ULabEquipmentDefinition> EquipmentDefinition)
{
	check(OwnerComponent);
	check(EquipmentDefinition != nullptr)
	check(OwnerComponent->GetOwner()->HasAuthority())
	
	ULabEquipmentInstance* NewInstance = nullptr;
	
	FLabEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	const ULabEquipmentDefinition* DefaultObject = GetDefault<ULabEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<ULabEquipmentInstance> InstanceType = DefaultObject->InstanceType;
	if (InstanceType == nullptr)
	{
		//未配置InstanceType则用默认的基类
		InstanceType = ULabEquipmentInstance::StaticClass();
	}
	//Using the actor instead of component as the outer due to UE-127172
	NewInstance = NewObject<ULabEquipmentInstance>(OwnerComponent->GetOuter(), InstanceType);
	
	NewEntry.Definition = EquipmentDefinition;
	NewEntry.Instance = NewInstance;
	
	if (ULabAbilitySystemComponent* LabASC = GetAbilitySystemComponent())
	{
		for (const auto& AbilitySetToGrant : DefaultObject->AbilitySetsToGrant)
		{
			AbilitySetToGrant->GiveToAbilitySystem(LabASC, &NewEntry.GrantedHandles, NewInstance);
		}
	}

	//Let the Instance Create Actors 
	NewInstance->SpawnEquipmentActors(DefaultObject->EquipmentActors);
	
	MarkItemDirty(NewEntry);
	return NewInstance;	
}

void FLabEquipmentList::RemoveEntry(ULabEquipmentInstance* InInstance)
{
	for (auto Iterator = Entries.CreateIterator(); Iterator; ++Iterator)
	{
		FLabEquipmentEntry& Entry = *Iterator;
		if (Entry.Instance == InInstance)
		{
			
			Entry.Instance->DestroyEquipmentActors();
			
			Iterator.RemoveCurrent();
			MarkArrayDirty();
			return;
		}
	}
}

ULabEquipmentManagerComponent::ULabEquipmentManagerComponent()
	: EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
}

void ULabEquipmentManagerComponent::GetDefaultEquipment()
{
	EquipmentList.AddEntry(EquipmentDefinition);
}

void ULabEquipmentManagerComponent::EquipItem(TSubclassOf<ULabEquipmentDefinition> InEquipmentDefinition)
{
}

void ULabEquipmentManagerComponent::UnequipItem(ULabEquipmentInstance* InEquipmentInstance)
{
	
}

void ULabEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

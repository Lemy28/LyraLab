// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/LabAbilitySet.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "LabEquipmentManagerComponent.generated.h"

class ULabEquipmentInstance;
class ULabEquipmentDefinition;
class ULabEquipmentManagerComponent;
// struct FLabAbilitySet_GrantedHandles;
USTRUCT()
struct FLabEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TSubclassOf<ULabEquipmentDefinition> Definition = nullptr;

	UPROPERTY()
	TObjectPtr<ULabEquipmentInstance> Instance = nullptr;

	UPROPERTY(NotReplicated)
	FLabAbilitySet_GrantedHandles GrantedHandles;
	FString GetDebugString();
};

USTRUCT()
struct FLabEquipmentList: public FFastArraySerializer
{
	GENERATED_USTRUCT_BODY()
public:
	FLabEquipmentList() : OwningComponent(nullptr) {}
	FLabEquipmentList(ULabEquipmentManagerComponent* InComp) : OwningComponent(InComp) {}
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FLabEquipmentEntry, FLabEquipmentList>( Entries, DeltaParms, *this );
	}

	void AddEntry(TSubclassOf<ULabEquipmentDefinition> EquipmentDefinition);
private:
	UPROPERTY()
	TArray<FLabEquipmentEntry>	Entries;

	UPROPERTY(NotReplicated)
	ULabEquipmentManagerComponent* OwningComponent;
};

template<>
struct TStructOpsTypeTraits< FLabEquipmentList > : public TStructOpsTypeTraitsBase2< FLabEquipmentList >
{
	enum 
	{
		WithNetDeltaSerializer = true,
   };
};

UCLASS()
class LYRALAB_API ULabEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULabEquipmentManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(Replicated)
	FLabEquipmentList EquipmentList;
};

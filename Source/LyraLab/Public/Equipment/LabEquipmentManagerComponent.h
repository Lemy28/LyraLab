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
	FLabEquipmentList() : OwnerComponent(nullptr) {}
	FLabEquipmentList(ULabEquipmentManagerComponent* InComp) : OwnerComponent(InComp) {}

	ULabAbilitySystemComponent* GetAbilitySystemComponent() const;
	
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	bool NetDeltaSerialize(FNetDeltaSerializeInfo & DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FLabEquipmentEntry, FLabEquipmentList>( Entries, DeltaParms, *this );
	}

	ULabEquipmentInstance* AddEntry(TSubclassOf<ULabEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(ULabEquipmentInstance* InInstance);
private:
	UPROPERTY()
	TArray<FLabEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	ULabEquipmentManagerComponent* OwnerComponent;
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
	//test
	// UFUNCTION(BlueprintCallable, Category = "Lab|Equipment")
	// void GetDefaultEquipment();
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lab|Equipment")
	// TSubclassOf<ULabEquipmentDefinition> EquipmentDefinition;


	void EquipItem(TSubclassOf<ULabEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(ULabEquipmentInstance* EquipmentInstance);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(Replicated)
	FLabEquipmentList EquipmentList;
};

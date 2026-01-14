// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "LabInventoryManagerComponent.generated.h"

class UInventoryFragment_Stackable;
class ULabInventoryItemInstance;
class ULabInventoryItemDefinition;
class ULabInventoryManagerComponent;

/** A single entry in an inventory */
USTRUCT(BlueprintType)
struct FLabInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ULabInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY()
	TSubclassOf<ULabInventoryItemDefinition> Definition = nullptr;
	
	// UPROPERTY(NotReplicated)
	// int32 LastObservedCount = INDEX_NONE;

/** 
 * Optional functions you can implement for client side notification of changes to items; 
 * Parameter type can match the type passed as the 2nd template parameter in associated call to FastArrayDeltaSerialize
 * 
 * NOTE: It is not safe to modify the contents of the array serializer within these functions, nor to rely on the contents of the array 
 * being entirely up-to-date as these functions are called on items individually as they are updated, and so may be called in the middle of a mass update.
 */
	// void PreReplicatedRemove(const struct FLabInventoryList& InArraySerializer);
	// void PostReplicatedAdd(const struct FLabInventoryList& InArraySerializer);
	// void PostReplicatedChange(const struct FLabInventoryList& InArraySerializer);

	// Optional: debug string used with LogNetFastTArray logging
	// FString GetDebugString();
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct FLabInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()
	
	FLabInventoryList() {}
	FLabInventoryList(ULabInventoryManagerComponent* OwnerComponent)
		: OwnerComponent(OwnerComponent){}
	
	UPROPERTY()
	TArray<FLabInventoryEntry> Entries;
	ULabInventoryItemInstance* AddEntry(TSubclassOf<ULabInventoryItemDefinition> ItemDefinition, int32 StackCount);
	int32 StackEntry(TSubclassOf<ULabInventoryItemDefinition> ItemDefinition, const UInventoryFragment_Stackable* StackableFragment);

	void AddEntry(ULabInventoryItemInstance* InventoryItemInstance);
    void RemoveEntry(ULabInventoryItemInstance* InventoryItemInstance);

    // 接管网络序列化 只要引擎试图同步这个 Struct，就会进这个函数
    bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
    {
        // 模板参数 <Item类型, List类型> 不能填错
        return FFastArraySerializer::FastArrayDeltaSerialize<FLabInventoryEntry, FLabInventoryList>(Entries, DeltaParms, *this);
    }

	UPROPERTY()
	ULabInventoryManagerComponent* OwnerComponent = nullptr;
};

/** 3. 开启自定义同步特性的开关 */
//Unreal 为了极致的性能，抛弃了“动态多态（Virtual）”，采用了“静态多态（Traits/模板）”。
template<>
struct TStructOpsTypeTraits<FLabInventoryList> : public TStructOpsTypeTraitsBase2<FLabInventoryList>
{
    enum
    {
        // 告诉 UE：这个结构体里有 NetDeltaSerialize 函数，请调用它！
        // 如果不设为 true，上面的代码完全不会生效，它会退化成普通数组同步。
        WithNetDeltaSerializer = true,
    };
};

/**
 * Component that manages an inventory of items
 */
UCLASS(Blueprintable)
class LYRALAB_API ULabInventoryManagerComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    ULabInventoryManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	ULabInventoryItemInstance* AddItemDefinition(TSubclassOf<ULabInventoryItemDefinition> ItemDef, int32 StackCount = 1);
private:
	UPROPERTY(Replicated)
	FLabInventoryList InventoryList;
};




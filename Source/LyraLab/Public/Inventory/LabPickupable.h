// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LabPickupable.generated.h"

class ULabInventoryItemDefinition;
class ULabInventoryManagerComponent;
class ULabInventoryItemInstance;

// USTRUCT(BlueprintType)
// struct FPickupTemplate
// {
// 	GENERATED_BODY()
//
// public:
// 	UPROPERTY(EditAnywhere)
// 	int32 StackCount = 1;
//
// 	UPROPERTY(EditAnywhere)
// 	TSubclassOf<ULabInventoryItemDefinition> ItemDef;
// };

USTRUCT(BlueprintType)
struct FPickupInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<ULabInventoryItemInstance> Item = nullptr;
};

USTRUCT(BlueprintType)
struct FInventoryPickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPickupInstance> Instances;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<ULabInventoryItemDefinition>> Definitions;
};

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class ULabPickupable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LYRALAB_API ILabPickupable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual FInventoryPickup GetPickup() const = 0;
};

UCLASS()
class ULabPickupableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure)
	TScriptInterface<ILabPickupable> GetPickupableFromActor(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void AddPickupableToInventory(ULabInventoryManagerComponent* InventoryManagerComponent, TScriptInterface<ILabPickupable> LabPickupable);
};
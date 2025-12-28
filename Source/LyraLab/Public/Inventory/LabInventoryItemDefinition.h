// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LabInventoryItemDefinition.generated.h"

class ULabInventoryItemInstance;
UCLASS(MinimalAPI, Abstract, EditInlineNew, DefaultToInstanced)
class ULabInventoryFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(ULabInventoryItemInstance* NewInstance) const {}
};


/**
 * 
 */
UCLASS()
class LYRALAB_API ULabInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FText DisplayName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")//, Instanced?
	TArray<TObjectPtr<ULabInventoryFragment>> Fragments;
};

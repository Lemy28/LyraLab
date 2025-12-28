// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LabInventoryItemInstance.generated.h"

struct FLabInventoryList;
class ULabInventoryItemDefinition;
/**
 * Instance class used by inventory component
 */
UCLASS()
class LYRALAB_API ULabInventoryItemInstance : public UObject
{
	GENERATED_BODY()

	
private:
	void SetItemDef(TSubclassOf<ULabInventoryItemDefinition> InItemDefinition) {ItemDefinition = InItemDefinition;}
	friend FLabInventoryList;
private:
	
	TSubclassOf<ULabInventoryItemDefinition> ItemDefinition;
};

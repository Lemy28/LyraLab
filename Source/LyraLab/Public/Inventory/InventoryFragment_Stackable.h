// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LabInventoryItemDefinition.h"
#include "InventoryFragment_Stackable.generated.h"

USTRUCT(BlueprintType)
struct FInventoryStackDefinition
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 StackCount;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 MaxStackCount;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 TotalStackCount = 999;
};

/**
 * This class is used for extending the behavior of lyra starter game's inventory stack logic 
 */
UCLASS()
class LYRALAB_API UInventoryFragment_Stackable : public ULabInventoryFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FInventoryStackDefinition StackDefinition;
};

// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LabPickupable.h"
#include "GameFramework/Actor.h"
#include "Interaction/LyraLabInteractable.h"
#include "LabWorldPickupable.generated.h"

UCLASS()
class LYRALAB_API ALabWorldPickupable : public ALyraLabInteractable, public ILabPickupable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALabWorldPickupable();
	UFUNCTION()
	virtual FInventoryPickup GetPickup() const override;
	
private:
	UPROPERTY(EditAnywhere)
	FInventoryPickup StaticInventory;
};

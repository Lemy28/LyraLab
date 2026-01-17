// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableTarget.h"
#include "GameFramework/Actor.h"
#include "LabInteractable.generated.h"

UCLASS()
class LYRALAB_API ALabInteractable : public AActor, public IInteractableTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALabInteractable();

	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
protected:
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;
public:

};

// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableTarget.h"
#include "GameFramework/Actor.h"
#include "LyraLabInteractable.generated.h"

UCLASS()
class LYRALAB_API ALyraLabInteractable : public AActor, public IInteractableTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALyraLabInteractable();

	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
protected:
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;
public:

};

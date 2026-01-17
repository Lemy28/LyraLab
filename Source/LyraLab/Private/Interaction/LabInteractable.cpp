// Copyright Lemy. All Rights Reserved.


#include "Interaction/LabInteractable.h"


// Sets default values
ALabInteractable::ALabInteractable()
{
}

void ALabInteractable::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(Option);
}

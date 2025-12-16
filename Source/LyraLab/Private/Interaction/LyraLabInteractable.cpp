// Copyright Lemy. All Rights Reserved.


#include "Interaction/LyraLabInteractable.h"


// Sets default values
ALyraLabInteractable::ALyraLabInteractable()
{
}

void ALyraLabInteractable::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(Option);
}

// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.h"
#include "UObject/Interface.h"
#include "InteractableTarget.generated.h"

struct FInteractionQuery;

class FInteractionOptionBuilder
{
public:
	FInteractionOptionBuilder(TScriptInterface<IInteractableTarget> InterfaceTargetScope, TArray<FInteractionOption>& InteractOptions)
		: Scope(InterfaceTargetScope)
		, Options(InteractOptions)
	{
		
	}
	void AddInteractionOption(const FInteractionOption& Option)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Add Interaction Option!!"))
		FInteractionOption& OptionEntry = Options.Add_GetRef(Option);
		OptionEntry.InteractableTarget = Scope;
	}
private:
	TArray<FInteractionOption>& Options; //
	TScriptInterface<IInteractableTarget> Scope;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LYRALAB_API IInteractableTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) = 0;
	
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutData) { }
};

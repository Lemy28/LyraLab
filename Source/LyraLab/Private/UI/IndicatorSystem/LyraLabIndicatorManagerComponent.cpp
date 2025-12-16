// Copyright Lemy. All Rights Reserved.


#include "UI/IndicatorSystem/LyraLabIndicatorManagerComponent.h"

#include "UI/IndicatorSystem/IndicatorDescriptor.h"


// Sets default values for this component's properties
ULyraLabIndicatorManagerComponent::ULyraLabIndicatorManagerComponent()
{
	bAutoActivate = true;
	bAutoRegister = true;
}

ULyraLabIndicatorManagerComponent* ULyraLabIndicatorManagerComponent::GetComponent(AController* Controller)
{
	if (Controller)
	{
		return Controller->FindComponentByClass<ULyraLabIndicatorManagerComponent>();
	}

	return nullptr;
}

void ULyraLabIndicatorManagerComponent::RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor)
{
	if (IndicatorDescriptor)
	{
		ensure(IndicatorDescriptor->GetIndicatorManagerComponent() == this);
	
		OnIndicatorRemoved.Broadcast(IndicatorDescriptor);
		Indicators.Remove(IndicatorDescriptor);
	}
}

void ULyraLabIndicatorManagerComponent::AddIndicator(UIndicatorDescriptor* IndicatorDescriptor)
{
	IndicatorDescriptor->SetIndicatorManagerComponent(this);
	OnIndicatorAdded.Broadcast(IndicatorDescriptor);
	Indicators.Add(IndicatorDescriptor);
}


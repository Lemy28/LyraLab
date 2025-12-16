// Copyright Lemy. All Rights Reserved.


#include "AbilitySystem/Attributes/LabHealthAttributeSet.h"

#include "Net/UnrealNetwork.h"

ULabHealthSet::ULabHealthSet()
{
	InitHealth(200);
	InitMaxHealth(200);
}

void ULabHealthSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Health)
	DOREPLIFETIME(ThisClass, MaxHealth)
}




void ULabHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	
}

void ULabHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	
}

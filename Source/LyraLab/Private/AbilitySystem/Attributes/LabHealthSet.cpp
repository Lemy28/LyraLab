// Copyright Lemy. All Rights Reserved.


#include "AbilitySystem/Attributes/LabHealthAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "LabHealthSet.h"

ULabHealthSet::ULabHealthSet()
{
	InitMaxHealth(1000);
	InitHealth(1000);
}

void ULabHealthSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Health)
	DOREPLIFETIME(ThisClass, MaxHealth)
}

void ULabHealthSet::PreBaseAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) const
{
	Super::PreBaseAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void ULabHealthSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void ULabHealthSet::PostAttributeChange(const FGameplayAttribute &Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		if (GetHealth() > NewValue)
		{
			// SetHealth(NewValue); why not use SetHealth directly?

			ULabAbilitySystemComponent* LabASC = GetLabAbilitySystemComponent();
			check(LabASC);
			LabASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

}

void ULabHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	
}

void ULabHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	
}

void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}
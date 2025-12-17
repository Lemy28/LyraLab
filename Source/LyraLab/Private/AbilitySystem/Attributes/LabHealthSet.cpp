// Copyright Lemy. All Rights Reserved.


#include "AbilitySystem/Attributes/LabHealthSet.h"

#include "AbilitySystem/LabAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
// #include "LabHealthSet.h"

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

void ULabHealthSet::PreAttributeBaseChange(const FGameplayAttribute &Attribute, float &NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

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
			// SetHealth(NewValue); why not use SetHealth directly?  SetNumericAttributeBase?
			//well, this is almost the same thing
			ULabAbilitySystemComponent* LabASC = GetLabAbilitySystemComponent();
			check(LabASC);

			// this modifier directly mods the base value instead of that typical GE channel
			LabASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		
		}
		
	}

}

void ULabHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULabHealthSet, Health, OldValue);
	
}

void ULabHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULabHealthSet, Health, OldValue);
}

void ULabHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
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
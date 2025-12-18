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

bool ULabHealthSet::PreGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
	if (Super::PreGameplayEffectExecute(Data) == false)
	{
		return false;
	}

	//handle Damage and Healing meta attributes with state like immune, suicide, etc here


	// Save the current health
	HealthBeforeAttributeChange = GetHealth();
	MaxHealthBeforeAttributeChange = GetMaxHealth();

    return true;
}

void ULabHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f
	//Consume the meta attribute

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			// Convert into -Health and then clamp
			SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		}

		SetDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			// Convert into +Health and then clamp
			SetHealth(FMath::Clamp(GetHealth() + GetDamage(), MinimumHealth, GetMaxHealth()));
		}

		SetHealing(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribtue())
	{
		// Clamp and fall into out of health handling below
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		// OnMaxHealthChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, MaxHealthBeforeAttributeChange, GetMaxHealth());
	}

	if (GetMaxHealth() != MaxHealthBeforeAttributeChange)
	{
		OnMaxHealthChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, MaxHealthBeforeAttributeChange, GetMaxHealth());
	}
	// If health has actually changed activate callbacks 
	if (GetHealth() != HealthBeforeAttributeChange)
	{
		OnHealthChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
	}

	// Handle out of health on Server
	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		OnOutOfHealth.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, HealthBeforeAttributeChange, GetHealth());
	}

	// Check health again in case an event above changed it.
	bOutOfHealth = (GetHealth() <= 0.0f);
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
//Add boilerplate Repnotify and broadcast HealthEvent

void ULabHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULabHealthSet, Health, OldValue);

	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldValue.GetCurrentValue();
	
	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);

	if (!bOutOfHealth && CurrentHealth <= 0.0f)
	{
		OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);
	}

	bOutOfHealth = (CurrentHealth <= 0.0f);
}

void ULabHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULabHealthSet, MaxHealth, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future   
	OnMaxHealthChanged.Broadcast(nullptr, nullptr, nullptr, GetMaxHealth() - OldValue.GetCurrentValue(), OldValue.GetCurrentValue(), GetMaxHealth());
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
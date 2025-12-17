// Copyright Lemy. All Rights Reserved.


#include "Character/LabHealthComponent.h"

#include "LabLogChannels.h"
#include "AbilitySystem/LabAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LabHealthSet.h"


// Sets default values for this component's properties
ULabHealthComponent::ULabHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
}

void ULabHealthComponent::InitializeWithAbilitySystem(ULabAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogLab, Error, TEXT("LabHealthComponent: Health component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}
	
	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogLab, Error, TEXT("LabHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	HealthSet = AbilitySystemComponent->GetSet<ULabHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogLab, Error, TEXT("LabHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}
	
	HealthSet->OnHealthChanged.AddLambda(
		[this](AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
		{
			this->OnHealthChanged.Broadcast(this, DamageInstigator, OldValue, NewValue);
		}
		);
	
	HealthSet->OnMaxHealthChanged.AddLambda(
	[this](AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
		{
			this->OnMaxHealthChanged.Broadcast(this, DamageInstigator, OldValue, NewValue);
		}
		);
	
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);

	// TEMP: Reset attributes to default values.  Eventually this will be driven by a spread sheet.
	AbilitySystemComponent->SetNumericAttributeBase(ULabHealthSet::GetHealthAttribute(), HealthSet->GetMaxHealth());
	OnHealthChanged.Broadcast(this, nullptr, HealthSet->GetHealth(), HealthSet->GetHealth());
	OnMaxHealthChanged.Broadcast(this,nullptr, HealthSet->GetHealth(), HealthSet->GetHealth());
}
void ULabHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser,
	const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	// OnOutOfHealth.Broadcast(this, DamageInstigator, OldValue, NewValue);
	
}
// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LabHealthComponent.generated.h"

struct FGameplayEffectSpec;
class ULabAbilitySystemComponent;
class ULabHealthSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHealthChangedEvent, ULabHealthComponent*, HealthComponent, AActor*, Instigator, float, OldValue, float, NewValue)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LYRALAB_API ULabHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULabHealthComponent();

	static ULabHealthComponent* FindComponent(const AActor* Actor){return Actor ? Actor->FindComponentByClass<ULabHealthComponent>() : nullptr;}
	void InitializeWithAbilitySystem(ULabAbilitySystemComponent* InASC);
	
public:
	UPROPERTY(BlueprintAssignable)
	FHealthChangedEvent OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FHealthChangedEvent OnMaxHealthChanged;

protected:
	void HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue);

protected:
	UPROPERTY()
	TObjectPtr<ULabAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<const ULabHealthSet> HealthSet;
};

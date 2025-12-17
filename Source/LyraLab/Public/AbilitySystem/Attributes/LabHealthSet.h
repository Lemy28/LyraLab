// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LabAttributeSet.h"
#include "LabHealthSet.generated.h"

/**
 * 
 */
UCLASS()
class LYRALAB_API ULabHealthSet : public ULabAttributeSet
{
	GENERATED_BODY()

public:
	ULabHealthSet();
	ATTRIBUTE_ACCESSORS_BASIC(ULabHealthSet, Health);
	ATTRIBUTE_ACCESSORS_BASIC(ULabHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(ULabHealthSet, Damage);
	ATTRIBUTE_ACCESSORS_BASIC(ULabHealthSet, Healing);

	//mutable for subscribers
	mutable FLabAttributeEvent OnHealthChanged;
	mutable FLabAttributeEvent OnMaxHealthChanged;
	mutable FLabAttributeEvent OnOutOfHealth;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:

	virtual void PreBaseAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;



	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue);

private:
	
	UPROPERTY(BlueprintReadOnly, Category = "Lab|Health", ReplicatedUsing = OnRep_Health, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, Category = "Lab|Health", ReplicatedUsing = OnRep_MaxHealth, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	
	//meta attribute on server
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lab|Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lab|Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;
protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
};

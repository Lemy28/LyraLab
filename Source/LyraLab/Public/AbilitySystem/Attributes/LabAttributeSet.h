// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "LabAttributeSet.generated.h"


struct FGameplayEffectSpec;
class ULabAbilitySystemComponent;
/** 
 * Delegate used to broadcast attribute events, some of these parameters may be null on clients: 
 * @param EffectInstigator	The original instigating actor for this event
 * @param EffectCauser		The physical actor that caused the change
 * @param EffectSpec		The full effect spec for this change
 * @param EffectMagnitude	The raw magnitude, this is before clamping
 * @param OldValue			The value of the attribute before it was changed
 * @param NewValue			The value after it was changed
*/
DECLARE_MULTICAST_DELEGATE_SixParams(FLabAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec* /*EffectSpec*/, float /*EffectMagnitude*/, float /*OldValue*/, float /*NewValue*/);


/**
 * base AttributeSet used by this project
 */
UCLASS()
class LYRALAB_API ULabAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	ULabAttributeSet();

protected:
	ULabAbilitySystemComponent* GetLabAbilitySystemComponent() const;
};

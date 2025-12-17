// Copyright Lemy. All Rights Reserved.


#include "AbilitySystem/Attributes/LabAttributeSet.h"
// #include "LabAttributeSet.h"

#include "AbilitySystem/LyraLabAbilitySystemComponent.h"

ULabAttributeSet::ULabAttributeSet()
{
}

ULabAbilitySystemComponent *ULabAttributeSet::GetLabAbilitySystemComponent() const
{
    return CastChecked<ULabAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

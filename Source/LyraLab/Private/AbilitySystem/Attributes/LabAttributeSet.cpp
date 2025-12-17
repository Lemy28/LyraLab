// Copyright Lemy. All Rights Reserved.


#include "AbilitySystem/Attributes/LabAttributeSet.h"
#include "LabAttributeSet.h"

ULabAbilitySystemComponent *ULabAttributeSet::GetLabAbilitySystemComponent() const
{
    return CastChecked<ULabAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
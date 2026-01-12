// Copyright Lemy. All Rights Reserved.


#include "Inventory/LabInventoryItemDefinition.h"

const ULabInventoryFragment* ULabInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<ULabInventoryFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (ULabInventoryFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}

const ULabInventoryFragment* ULabInventoryFunctionLibrary::FindItemDefinitionFragment(
	TSubclassOf<ULabInventoryItemDefinition> ItemDef, TSubclassOf<ULabInventoryFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<ULabInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}


// Copyright Lemy. All Rights Reserved.


#include "Inventory/LabWorldPickupable.h"


// Sets default values
ALabWorldPickupable::ALabWorldPickupable()
{

}

FInventoryPickup ALabWorldPickupable::GetPickup() const
{
	return Pickup;
}

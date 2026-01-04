// Copyright Lemy. All Rights Reserved.


#include "Equipment/LabEquipmentInstance.h"

#include "Equipment/LabEquipmentDefinition.h"
#include "GameFramework/Character.h"

ULabEquipmentInstance::ULabEquipmentInstance()
{
}

void ULabEquipmentInstance::SpawnEquipmentActors(const TArray<FLabEquipmentActorToSpawn>& EquipmentActors)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* TargetComponent = OwningPawn->GetRootComponent();
		if (ACharacter* Character = Cast<ACharacter>(OwningPawn))
		{
			TargetComponent = Character->GetMesh();
		}
		
		for (const auto& EquipmentActorToSpawn : EquipmentActors)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(EquipmentActorToSpawn.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, true);
			NewActor->SetActorRelativeTransform(EquipmentActorToSpawn.AttachTransform);
			NewActor->AttachToComponent(TargetComponent, FAttachmentTransformRules::KeepRelativeTransform, EquipmentActorToSpawn.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void ULabEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* SpawnedActor : SpawnedActors)
	{
		if (SpawnedActor != nullptr)
		{
			SpawnedActor->Destroy();
		}
	}
}

void ULabEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void ULabEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

APawn* ULabEquipmentInstance::GetPawn()
{
	return Cast<APawn>(GetOuter());
}

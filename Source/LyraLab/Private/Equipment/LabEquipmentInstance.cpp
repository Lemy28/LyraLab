// Copyright Lemy. All Rights Reserved.


#include "Equipment/LabEquipmentInstance.h"

#include "Equipment/LabEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

ULabEquipmentInstance::ULabEquipmentInstance()
{
}
UWorld* ULabEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	
	return nullptr;
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

void ULabEquipmentInstance::OnRep_Instigator()
{
}

APawn* ULabEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void ULabEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}
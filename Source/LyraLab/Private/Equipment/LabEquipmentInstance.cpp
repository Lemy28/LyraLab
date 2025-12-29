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
			FActorSpawnParameters SpawnParams;
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(EquipmentActorToSpawn.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, true);
			NewActor->SetActorRelativeTransform(EquipmentActorToSpawn.AttachTransform);
			NewActor->AttachToComponent(TargetComponent, FAttachmentTransformRules::KeepRelativeTransform, EquipmentActorToSpawn.AttachSocket);

			SpawnedActors.Add(NewActor);
			
		}
		
	}
	

}

APawn* ULabEquipmentInstance::GetPawn()
{
	return Cast<APawn>(GetOuter());
}

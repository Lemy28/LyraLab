// Copyright Lemy. All Rights Reserved.


#include "Animation/LyraLabAnimInstance.h"

#include "Character/LabCharacter.h"
#include "Character/LyraLabCharacterMovementComponent.h"

void ULyraLabAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ALabCharacter* Character = Cast<ALabCharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	ULyraLabCharacterMovementComponent* CharMoveComp = CastChecked<ULyraLabCharacterMovementComponent>(Character->GetCharacterMovement());
	const FLyraCharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}

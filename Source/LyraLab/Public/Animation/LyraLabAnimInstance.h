// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LyraLabAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LYRALAB_API ULyraLabAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

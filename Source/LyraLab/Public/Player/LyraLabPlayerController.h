// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LyraLabPlayerController.generated.h"
class ULabAbilitySystemComponent;
class ALabPlayerState;
/**
 * 
 */
UCLASS()
class LYRALAB_API ALyraLabPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerController")
	ULabAbilitySystemComponent* GetLyraAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerController")
	ALabPlayerState* GetLyraPlayerState() const;
	//~ Begin APlayerController interface
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~ End APlayerController interface
	virtual void AcknowledgePossession(class APawn* P) override;
};

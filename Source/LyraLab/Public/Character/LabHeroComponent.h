// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "LabHeroComponent.generated.h"

class ULabInputConfig;
struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LYRALAB_API ULabHeroComponent : public UActorComponent//, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	ULabHeroComponent();
	//~ Begin IGameFrameworkInitStateInterface interface
	//virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	//~ End IGameFrameworkInitStateInterface interface

	/** Returns the hero component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Lyra|Hero")
	static ULabHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULabHeroComponent>() : nullptr); }

	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
protected:

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<ULabInputConfig> InputConfig;
};

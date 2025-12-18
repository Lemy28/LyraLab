// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AlsCharacter.h"
#include "GameFramework/Character.h"
#include "LabCharacter.generated.h"

class ALabPlayerController;
class ALabPlayerState;
class ULabHealthComponent;
class UAlsCameraComponent;
class ULabHeroComponent;
class ULabInputConfig;
class ULabPawnExtensionComponent;
class ULabAbilitySystemComponent;

UCLASS()
class LYRALAB_API ALabCharacter : public AAlsCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ALabCharacter();
	
	ALabPlayerState* GetLabPlayerState();
	ALabPlayerController* GetLabPlayerController();
	ULabAbilitySystemComponent* GetLabAbilitySystemComponent() const ;
	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override;
	
	//~ Begin APawn Interface.
	virtual void PawnClientRestart() override;
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.
	//~ Begin ACharacter Interface.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//~ End ACharacter Interface.

	virtual void PostInitializeComponents() override;
	
protected:
	virtual void OnAbilitySystemInitialized();
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character|Camera")
	TObjectPtr<UAlsCameraComponent> Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<ULabHeroComponent> HeroComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<ULabHealthComponent> HealthComponent;
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULabPawnExtensionComponent> PawnExtComponent;


	
	//Camera
	
protected:
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& ViewInfo) override;

	//Debug
	
public:
	
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused, float& VerticalLocation) override;


};

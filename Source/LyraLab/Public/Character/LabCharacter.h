// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AlsCharacter.h"
#include "GameFramework/Character.h"
#include "LabCharacter.generated.h"

class ULabHealthComponent;
class UAlsCameraComponent;
class ULabHeroComponent;
class ULabInputConfig;
class ULabPawnExtensionComponent;


/**
 * FLyraReplicatedAcceleration: Compressed representation of acceleration
 */
USTRUCT()
struct FLyraReplicatedAcceleration
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 AccelXYRadians = 0;	// Direction of XY accel component, quantized to represent [0, 2*pi]

	UPROPERTY()
	uint8 AccelXYMagnitude = 0;	//Accel rate of XY component, quantized to represent [0, MaxAcceleration]

	UPROPERTY()
	int8 AccelZ = 0;	// Raw Z accel rate component, quantized to represent [-MaxAcceleration, MaxAcceleration]
};


UCLASS()
class LYRALAB_API ALabCharacter : public AAlsCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ALabCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin AActor Interface.
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	//~ End AActor Interface.

	//~ Begin APawn Interface.
	virtual void PawnClientRestart() override;
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface.
	//~ Begin ACharacter Interface.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//~ End ACharacter Interface.

	virtual void PostInitializeComponents() override;
	
protected:
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
	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const;
	
	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused, float& VerticalLocation) override;
private:
// 	UPROPERTY(Transient, ReplicatedUsing = OnRep_ReplicatedAcceleration)
// 	FLyraReplicatedAcceleration ReplicatedAcceleration;
//
//
// public:
// 	UFUNCTION()
// 	void OnRep_ReplicatedAcceleration();
};

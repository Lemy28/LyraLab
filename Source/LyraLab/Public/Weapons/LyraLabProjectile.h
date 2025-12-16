// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LyraLabProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class LYRALAB_API ALyraLabProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALyraLabProjectile();

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleComp;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LabPawnExtensionComponent.generated.h"

class ULabAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LYRALAB_API ULabPawnExtensionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULabPawnExtensionComponent();
	/** Returns the pawn extension component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Lab|Pawn")
	static ULabPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULabPawnExtensionComponent>() : nullptr); }
	/** Gets the current ability system component, which may be owned by a different actor */
	UFUNCTION(BlueprintPure, Category = "Lab|Pawn")
	ULabAbilitySystemComponent* GetLyraAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	/** Should be called by the owning pawn to become the avatar of the ability system. */
	void InitializeAbilitySystem(ULabAbilitySystemComponent* InASC, AActor* InOwnerActor);
	
	/** Should be called by the owning pawn to remove itself as the avatar of the ability system. */
	void UninitializeAbilitySystem();


	// /** Register with the OnAbilitySystemInitialized delegate and broadcast if our pawn has been registered with the ability system component */
	// UE_API void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	//
	// /** Register with the OnAbilitySystemUninitialized delegate fired when our pawn is removed as the ability system's avatar actor */
	// UE_API void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);
protected:
	/** Delegate fired when our pawn becomes the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	/** Delegate fired when our pawn is removed as the ability system's avatar actor */
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;
public:


private:
	/** Pointer to the ability system component that is cached for convenience. */
	UPROPERTY(Transient)
	TObjectPtr<ULabAbilitySystemComponent> AbilitySystemComponent;
};

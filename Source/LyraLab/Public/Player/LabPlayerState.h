// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LabPlayerState.generated.h"

class ULabHealthSet;
class ULabAbilitySystemComponent;
class ULyraLabPawnData;
/** Defines the types of client connected */
UENUM()
enum class ELyraPlayerConnectionType : uint8
{
	// An active player
	Player = 0,

	// Spectator connected to a running game
	LiveSpectator,

	// Spectating a demo recording offline
	ReplaySpectator,

	// A deactivated player (disconnected)
	InactivePlayer
};
/**
 * 
 */
UCLASS()
class LYRALAB_API ALabPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ALabPlayerState();
	void SetPawnData(const ULyraLabPawnData* InPawnData);
	
	UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerState")
	ULabAbilitySystemComponent* GetLabAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface
protected:
	UFUNCTION()
	void OnRep_PawnData();
protected:
	
	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const ULyraLabPawnData> PawnData;

private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Lab|Ability")
	TObjectPtr<ULabAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Lab|Ability")
	TObjectPtr<ULabHealthSet> HealthSet;

	
    // UPROPERTY(Replicated)
    // ELyraPlayerConnectionType MyPlayerConnectionType;

	// UPROPERTY(Replicated)
	// FGameplayTagStackContainer StatTags;
};

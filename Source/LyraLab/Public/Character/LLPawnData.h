// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLPawnData.generated.h"

class ULabInputConfig;
class ULyraLabAbilitySet;
/**
 * 
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Lyra Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class LYRALAB_API ULyraLabPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	ULyraLabPawnData();
public:

	// Class to instantiate for this pawn (should usually derive from ALyraPawn or ALyraCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Abilities")
	TArray<TObjectPtr<ULyraLabAbilitySet>> AbilitySets;

	// // What mapping of ability tags to use for actions taking by this pawn
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Abilities")
	// TObjectPtr<ULyraAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Input")
	TObjectPtr<ULabInputConfig> InputConfig;

	// // Default camera mode used by player controlled pawns.
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Camera")
	// TSubclassOf<ULyraCameraMode> DefaultCameraMode;
};

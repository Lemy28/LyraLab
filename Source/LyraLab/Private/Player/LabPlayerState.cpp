// Copyright Lemy. All Rights Reserved.


#include "Player/LabPlayerState.h"

#include "LabLogChannels.h"
#include "AbilitySystem/LyraLabAbilitySet.h"
#include "AbilitySystem/LyraLabAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/LabHealthSet.h"
#include "Character/LLPawnData.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "VisualLogger/VisualLoggerKismetLibrary.h"


ALabPlayerState::ALabPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<ULabAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HealthSet = CreateDefaultSubobject<ULabHealthSet>(TEXT("HealthSet"));
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);
}

void ALabPlayerState::SetPawnData(const ULyraLabPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogLab, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const ULyraLabAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	//UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_LyraAbilityReady);
	
	ForceNetUpdate();
}

void ALabPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ALabPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());


	
	// UWorld* World = GetWorld();
	// if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	// {
	// 	AGameStateBase* GameState = GetWorld()->GetGameState();
	// 	check(GameState);
	// 	ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	// 	check(ExperienceComponent);
	// 	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	// }
}

void ALabPlayerState::OnRep_PawnData()
{
}

void ALabPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);

	//DOREPLIFETIME(ThisClass, StatTags);	
}
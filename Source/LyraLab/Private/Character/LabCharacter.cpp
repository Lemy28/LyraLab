// Copyright Lemy. All Rights Reserved.


#include "Character/LabCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "LabGameplayTags.h"
#include "AbilitySystem/LabAbilitySystemComponent.h"
// #include "ALSCamera/Public/AlsCameraComponent.h"
#include "AlsCameraComponent.h"
#include "Character/LabHealthComponent.h"
#include "Character/LyraLabCharacterMovementComponent.h"
#include "Character/LabHeroComponent.h"
#include "Character/LabPawnExtensionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameModes/LyraLabGameMode.h"
#include "Player/LabPlayerController.h"
#include "Input/LabInputComponent.h"
#include "Input/LabInputConfig.h"
#include "Net/UnrealNetwork.h"
#include "Player/LabPlayerState.h"


// Sets default values
ALabCharacter::ALabCharacter()
{
	Camera = CreateDefaultSubobject<UAlsCameraComponent>(FName{TEXTVIEW("Camera")});
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});

	PawnExtComponent = CreateDefaultSubobject<ULabPawnExtensionComponent>(TEXT("LabPawnExtComponent"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleDelegate::CreateUObject(this, &ALabCharacter::OnAbilitySystemInitialized));

	HeroComponent = CreateDefaultSubobject<ULabHeroComponent>(TEXT("LabHeroComponent"));
	HealthComponent = CreateDefaultSubobject<ULabHealthComponent>(TEXT("LabHealthComponent"));
}

ALabPlayerState* ALabCharacter::GetLabPlayerState()
{
	return CastChecked<ALabPlayerState>(GetPlayerState());
}

ALabPlayerController* ALabCharacter::GetLabPlayerController()
{
	return CastChecked<ALabPlayerController>(GetController());
}
UAbilitySystemComponent* ALabCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent ? PawnExtComponent->GetLabAbilitySystemComponent() : nullptr;
}

ULabAbilitySystemComponent* ALabCharacter::GetLabAbilitySystemComponent() const 
{
	return Cast<ULabAbilitySystemComponent>(GetAbilitySystemComponent());
}

void ALabCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
}

void ALabCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ALabPlayerState* PS = GetPlayerState<ALabPlayerState>();

	if (!ensure(PS))
	{
		return;
	}
	
	if (ULabAbilitySystemComponent* AbilitySystemComponent = PS->GetLabAbilitySystemComponent())
	{
		if (PawnExtComponent)
		{
			PawnExtComponent->InitializeAbilitySystem(AbilitySystemComponent, PS);
		}
	}
	
	if (ALyraLabGameMode* LyraGameMode = GetWorld()->GetAuthGameMode<ALyraLabGameMode>())
	{
		if (const ULyraLabPawnData* NewPawnData = LyraGameMode->GetPawnDataForController(GetController()))
		{
			PS->SetPawnData(NewPawnData);
		}
	}

}
//
// void ALabCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
// {
// 	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
// 	// DOREPLIFETIME_CONDITION(ThisClass, ReplicatedAcceleration, COND_SimulatedOnly);
// 	//DOREPLIFETIME(ThisClass, MyTeamID)
// }



void ALabCharacter::CalcCamera(float DeltaTime, FMinimalViewInfo& ViewInfo)
{
	if (Camera->IsActive())
	{
		Camera->GetViewInfo(ViewInfo);
		return;
	}
	
	Super::CalcCamera(DeltaTime, ViewInfo);
}

void ALabCharacter::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused,
	float& VerticalLocation)
{
	if (Camera->IsActive())
	{
		Camera->DisplayDebug(Canvas, DisplayInfo, VerticalLocation);
	}

	Super::DisplayDebug(Canvas, DisplayInfo, Unused, VerticalLocation);
}

// Called to bind functionality to input
void ALabCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(HeroComponent)
	HeroComponent->InitializePlayerInput(PlayerInputComponent);

}

// void ALabCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
// {
// 	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
// 	
// }

void ALabCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

void ALabCharacter::OnAbilitySystemInitialized()
{
	HealthComponent->InitializeWithAbilitySystem(GetLabAbilitySystemComponent());
}

//
// void ALabCharacter::OnRep_ReplicatedAcceleration()
// {
// 	if (ULyraLabCharacterMovementComponent* LyraMovementComponent = Cast<ULyraLabCharacterMovementComponent>(GetCharacterMovement()))
// 	{
// 		// Decompress Acceleration
// 		const double MaxAccel         = LyraMovementComponent->MaxAcceleration;
// 		const double AccelXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * MaxAccel / 255.0; // [0, 255] -> [0, MaxAccel]
// 		const double AccelXYRadians   = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0;     // [0, 255] -> [0, 2PI]
//
// 		FVector UnpackedAcceleration(FVector::ZeroVector);
// 		FMath::PolarToCartesian(AccelXYMagnitude, AccelXYRadians, UnpackedAcceleration.X, UnpackedAcceleration.Y);
// 		UnpackedAcceleration.Z = double(ReplicatedAcceleration.AccelZ) * MaxAccel / 127.0; // [-127, 127] -> [-MaxAccel, MaxAccel]
//
// 		LyraMovementComponent->SetReplicatedAcceleration(UnpackedAcceleration);
// 	}
// }
//

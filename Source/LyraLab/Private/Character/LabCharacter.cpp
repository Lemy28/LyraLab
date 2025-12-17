// Copyright Lemy. All Rights Reserved.


#include "Character/LabCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "LabGameplayTags.h"
#include "AbilitySystem/LyraLabAbilitySystemComponent.h"
// #include "ALSCamera/Public/AlsCameraComponent.h"
#include "AlsCameraComponent.h"
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
	// : Super(ObjectInitializer.SetDefaultSubobjectClass<ULyraLabCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// PrimaryActorTick.bCanEverTick = false;
	// PrimaryActorTick.bStartWithTickEnabled = false;
	//
	// SetNetCullDistanceSquared(900000000.0f);
	
	Camera = CreateDefaultSubobject<UAlsCameraComponent>(FName{TEXTVIEW("Camera")});
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});

	// UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	// check(CapsuleComp);
	// CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	// // CapsuleComp->SetCollisionProfileName(NAME_LyraCharacterCollisionProfile_Capsule);
	//
	// USkeletalMeshComponent* MeshComp = GetMesh();
	// check(MeshComp);
	// MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	// MeshComp->SetCollisionProfileName(NAME_LyraCharacterCollisionProfile_Mesh);

	// ULyraLabCharacterMovementComponent* LyraMoveComp = CastChecked<ULyraLabCharacterMovementComponent>(GetCharacterMovement());
	// LyraMoveComp->GravityScale = 1.0f;
	// // LyraMoveComp->MaxAcceleration = 2400.0f;
	// LyraMoveComp->BrakingFrictionFactor = 1.0f;
	// // LyraMoveComp->BrakingFriction = 6.0f;
	// // LyraMoveComp->GroundFriction = 8.0f;
	// // LyraMoveComp->BrakingDecelerationWalking = 1400.0f;
	// LyraMoveComp->bUseControllerDesiredRotation = false;
	// LyraMoveComp->bOrientRotationToMovement = false;
	// LyraMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	// LyraMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	// LyraMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	// LyraMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	// LyraMoveComp->SetCrouchedHalfHeight(65.0f);

	// PawnExtComponent = CreateDefaultSubobject<ULyraPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	// PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	// PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
	//
	// HealthComponent = CreateDefaultSubobject<ULyraHealthComponent>(TEXT("HealthComponent"));
	// HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
	// HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);
	//
	// CameraComponent = CreateDefaultSubobject<ULyraCameraComponent>(TEXT("CameraComponent"));
	// CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));


	HeroComponent = CreateDefaultSubobject<ULabHeroComponent>(TEXT("LyraLabHeroComponent"));
	//
	// bUseControllerRotationPitch = false;
	// bUseControllerRotationYaw = true;
	// bUseControllerRotationRoll = false;
	//
	// BaseEyeHeight = 80.0f;
	// CrouchedEyeHeight = 50.0f;
}

ALabPlayerState* ALabCharacter::GetLabPlayerState()
{
	return CastChecked<ALabPlayerState>(GetPlayerState());
}

ALabPlayerController* ALabCharacter::GetLabPlayerController()
{
	return CastChecked<ALabPlayerController>(GetController());
}

void ALabCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	// {
	// 	// Compress Acceleration: XY components as direction + magnitude, Z component as direct value
	// 	const double MaxAccel = MovementComponent->MaxAcceleration;
	// 	const FVector CurrentAccel = MovementComponent->GetCurrentAcceleration();
	// 	double AccelXYRadians, AccelXYMagnitude;
	// 	FMath::CartesianToPolar(CurrentAccel.X, CurrentAccel.Y, AccelXYMagnitude, AccelXYRadians);
	//
	// 	ReplicatedAcceleration.AccelXYRadians   = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);     // [0, 2PI] -> [0, 255]
	// 	ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
	// 	ReplicatedAcceleration.AccelZ           = FMath::FloorToInt((CurrentAccel.Z / MaxAccel) * 127.0);   // [-MaxAccel, MaxAccel] -> [-127, 127]
	// }
}

// Called when the game starts or when spawned
// void ALabCharacter::BeginPlay()
// {
// 	Super::BeginPlay();
//
//
// }

void ALabCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
}

void ALabCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ALabPlayerController* PC = CastChecked<ALabPlayerController>(NewController);
	ULabPawnExtensionComponent* PawnExtensionComponent = ULabPawnExtensionComponent::FindPawnExtensionComponent(this);
	ALabPlayerState* PS = GetPlayerState<ALabPlayerState>();

	if (!ensure(PS))
	{
		return;
	}
	
	if (ULabAbilitySystemComponent* AbilitySystemComponent = PC->GetLabAbilitySystemComponent())
	{
		if (PawnExtensionComponent)
		{
			PawnExtensionComponent->InitializeAbilitySystem(AbilitySystemComponent, PS);
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

UAbilitySystemComponent* ALabCharacter::GetAbilitySystemComponent() const
{
	if (PawnExtComponent == nullptr)
	{
		return nullptr;
	}
	return PawnExtComponent->GetLyraAbilitySystemComponent();
}

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

void ALabCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
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

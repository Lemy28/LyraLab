// Copyright Lemy. All Rights Reserved.


#include "Character/LabHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "LabGameplayTags.h"
// #include "LabLogChannels.h"
#include "AbilitySystem/LabAbilitySystemComponent.h"
#include "Character/LabCharacter.h"
#include "Character/LabPawnExtensionComponent.h"
#include "Player/LabPlayerController.h"
#include "Input/LabInputComponent.h"


ULabHeroComponent::ULabHeroComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

// void ULyraLabHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
// 	FGameplayTag DesiredState)
// {
// 	//test
// 	UE_LOG(LogTemp, Warning, TEXT("Test For HandleChangeInitState"));
// }

void ULabHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(InputConfig)
	ACharacter* Owner = GetOwner<ACharacter>();
	check(Owner)
	ALabPlayerController* LyraLabPlayerController = Owner->GetController<ALabPlayerController>();
	if (LyraLabPlayerController)
	{
		ULocalPlayer* LocalPlayer = LyraLabPlayerController->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem= LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (EnhancedInputSubsystem && InputConfig->DefaultInputMappingContext)
		{
			EnhancedInputSubsystem->RemoveMappingContext(InputConfig->DefaultInputMappingContext);
			EnhancedInputSubsystem->AddMappingContext(InputConfig->DefaultInputMappingContext, 0);
		}
	}
	
	ULabInputComponent* LyraLabInputComponent = CastChecked<ULabInputComponent>(PlayerInputComponent);
	if (LyraLabInputComponent)
	{
		TArray<uint32> BindHandles;
		LyraLabInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

		LyraLabInputComponent->BindNativeAction(InputConfig, LabGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this , &ThisClass::Input_Move,true);
		LyraLabInputComponent->BindNativeAction(InputConfig, LabGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this , &ThisClass::Input_LookMouse, true);
		LyraLabInputComponent->BindNativeAction(InputConfig, LabGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this , &ThisClass::Input_Crouch, true);
		LyraLabInputComponent->BindNativeAction(InputConfig, LabGameplayTags::InputTag_Jump, ETriggerEvent::Triggered, this , &ThisClass::Input_Jump, true);
	}
}

void ULabHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetOwner<APawn>())
	{
		if (const ULabPawnExtensionComponent* PawnExtComp = ULabPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (ULabAbilitySystemComponent* LyraASC = PawnExtComp->GetLabAbilitySystemComponent())
			{
				LyraASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void ULabHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetOwner<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const ULabPawnExtensionComponent* PawnExtComp = ULabPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (ULabAbilitySystemComponent* LyraASC = PawnExtComp->GetLabAbilitySystemComponent())
		{
			LyraASC->AbilityInputTagReleased(InputTag);
		}
	}
}


void ULabHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetOwner<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ULabHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetOwner<APawn>();
	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	// UE_LOG(LogLab, Log, TEXT("Look Input: %s"), *Value.ToString());
	
	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
		// UE_LOG(LogTemp, Log, TEXT("ULabHeroComponent Look Input: %f"), Value.Y);
	}
}

void ULabHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	AAlsCharacter* Character = GetOwner<AAlsCharacter>();
	if (!Character)
	{
		return;
	}
	
	if (Character->GetDesiredStance() == AlsStanceTags::Standing)
	{
		Character->SetDesiredStance(AlsStanceTags::Crouching);
	}
	else if (Character->GetDesiredStance() == AlsStanceTags::Crouching)
	{
		Character->SetDesiredStance(AlsStanceTags::Standing);
	}
}

void ULabHeroComponent::Input_Jump(const FInputActionValue& InputActionValue)
{
	AAlsCharacter* Character = GetOwner<AAlsCharacter>();
	if (!Character)
	{
		return;
	}
	
	if (InputActionValue.Get<bool>())
	{
		if (Character->GetStance() == AlsStanceTags::Crouching)
		{
			Character->SetDesiredStance(AlsStanceTags::Standing);
			return;
		}
		GetOwner<ACharacter>()->Jump();
	}
	else
	{
		GetOwner<ACharacter>()->StopJumping();
	}
}

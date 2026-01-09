// Copyright Epic Games, Inc. All Rights Reserved.

#include "TDWPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraFunctionLibrary.h"
#include "EnhancedInputComponent.h"
#include "TDWPlayerState.h"
#include "GameFramework/Pawn.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "TDW/TDWGameplayTags.h"
#include "TDW/AbilitySystem/TDWAbilitySystemComponent.h"
#include "TDW/Input/TDWInputComponent.h"
#include "TDW/Input/TDWInputConfig.h"
#include "UserSettings/EnhancedInputUserSettings.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


ATDWPlayerController::ATDWPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

ATDWPlayerState* ATDWPlayerController::GetTDWPlayerState() const
{
	return CastChecked<ATDWPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UTDWAbilitySystemComponent* ATDWPlayerController::GetTDWAbilitySystemComponent() const
{
	const auto* PS = GetTDWPlayerState();
	return (PS ? PS->GetTDWAbilitySystemComponent() : nullptr);
}

void ATDWPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ATDWPlayerController::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (auto* ASC = GetTDWAbilitySystemComponent())
	{
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void ATDWPlayerController::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (auto* ASC = GetTDWAbilitySystemComponent())
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void ATDWPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	auto* LP = GetLocalPlayer();
	check(LP);
	
	auto* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (InputConfig)
	{
		for (const FInputMappingContextAndPriority& Mapping : InputConfig->DefaultInputMappings)
		{
			if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
			{
				if (Mapping.bRegisterWithSettings)
				{
					if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
					{
						Settings->RegisterInputMappingContext(IMC);
					}
					
					FModifyContextOptions Options = {};
					Options.bIgnoreAllPressedKeysUntilRelease = false;
					// Actually add the config to the local player							
					Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
				}
			}
		}

		auto* TdwIC = Cast<UTDWInputComponent>(InputComponent);
		if (ensureMsgf(TdwIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs.")))
		{
			TArray<uint32> BindHandles;
			TdwIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
			
			TdwIC->BindNativeAction(InputConfig, TDWGameplayTags::Input_SetDestination, ETriggerEvent::Started, this, &ThisClass::Input_StopMovement, /*bLogIfNotFound=*/ true);
			TdwIC->BindNativeAction(InputConfig, TDWGameplayTags::Input_SetDestination, ETriggerEvent::Triggered, this, &ThisClass::Input_SetDestinationTriggered, /*bLogIfNotFound=*/ true);
			TdwIC->BindNativeAction(InputConfig, TDWGameplayTags::Input_SetDestination, ETriggerEvent::Completed, this, &ThisClass::Input_SetDestinationReleased, /*bLogIfNotFound=*/ true);
			TdwIC->BindNativeAction(InputConfig, TDWGameplayTags::Input_SetDestination, ETriggerEvent::Canceled, this, &ThisClass::Input_SetDestinationReleased, /*bLogIfNotFound=*/ true);
		}
	}
	
	/* TEMPLATE Code
	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ATDWPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ATDWPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ATDWPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ATDWPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ATDWPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ATDWPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ATDWPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ATDWPlayerController::OnTouchReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	*/
}

void ATDWPlayerController::Input_StopMovement()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void ATDWPlayerController::Input_SetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ATDWPlayerController::Input_SetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// TEMPLATE Code
// Triggered every frame when the input is held down
// void ATDWPlayerController::OnTouchTriggered()
// {
// 	bIsTouch = true;
// 	Input_SetDestinationTriggered();
// }
//
// void ATDWPlayerController::OnTouchReleased()
// {
// 	bIsTouch = false;
// 	Input_SetDestinationReleased();
// }

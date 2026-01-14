// Copyright Epic Games, Inc. All Rights Reserved.

#include "TDW/Character/TDWCharacter.h"

#include "TDWCombatComponent.h"
#include "TDWHealthComponent.h"
#include "TDWManaComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "TDW/AbilitySystem/TDWAbilitySet.h"
#include "TDW/AbilitySystem/TDWAbilitySystemComponent.h"
#include "TDW/Player/TDWPlayerState.h"

ATDWCharacter::ATDWCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CombatComponent = CreateDefaultSubobject<UTDWCombatComponent>(TEXT("CombatComponent"));
	HealthComponent = CreateDefaultSubobject<UTDWHealthComponent>(TEXT("HealthComponent"));
	ManaComponent = CreateDefaultSubobject<UTDWManaComponent>(TEXT("ManaComponent"));
}

void ATDWCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ATDWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeAbilityActorInfo();
	SetupAbilities();
}

void ATDWCharacter::UnPossessed()
{
	Super::UnPossessed();

	CombatComponent->UninitializeFromAbilitySystem();
	ManaComponent->UninitializeFromAbilitySystem();
}

void ATDWCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilityActorInfo();
}

void ATDWCharacter::InitializeAbilityActorInfo()
{
	auto* PS = GetPlayerState<ATDWPlayerState>();
	check(PS);

	AbilitySystemComponent = PS->GetTDWAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	
	CombatComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
	ManaComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
}

void ATDWCharacter::SetupAbilities()
{
	check(AbilitySystemComponent);
	
	AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, /** OutGrantedHandles*/ nullptr);
	//@Eric TODO: Store default abilities granted handles? 
}

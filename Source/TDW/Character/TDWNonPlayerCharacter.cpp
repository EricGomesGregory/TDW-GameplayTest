// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWNonPlayerCharacter.h"

#include "TDWCombatComponent.h"
#include "TDWHealthComponent.h"
#include "TDW/AbilitySystem/TDWAbilitySystemComponent.h"
#include "TDW/AbilitySystem/Attributes/TDWCombatSet.h"
#include "TDW/AbilitySystem/Attributes/TDWHealthSet.h"


ATDWNonPlayerCharacter::ATDWNonPlayerCharacter(const FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UTDWAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	HealthSet = CreateDefaultSubobject<UTDWHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<UTDWCombatSet>(TEXT("CombatSet"));

	SetNetUpdateFrequency(100.0f);

	HealthComponent = CreateDefaultSubobject<UTDWHealthComponent>(TEXT("HealthComponent"));
	CombatComponent = CreateDefaultSubobject<UTDWCombatComponent>(TEXT("CombatComponent"));
}

UAbilitySystemComponent* ATDWNonPlayerCharacter::GetAbilitySystemComponent() const
{
	return GetTDWAbilitySystemComponent();
}

void ATDWNonPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	
	HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
	CombatComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
}

void ATDWNonPlayerCharacter::PostUnregisterAllComponents()
{
	HealthComponent->UninitializeFromAbilitySystem();
	CombatComponent->UninitializeFromAbilitySystem();
	
	Super::PostUnregisterAllComponents();
}




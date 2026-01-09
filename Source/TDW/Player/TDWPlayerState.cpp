// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWPlayerState.h"

#include "TDW/AbilitySystem/TDWAbilitySystemComponent.h"
#include "TDW/AbilitySystem/Attributes/TDWCombatSet.h"
#include "TDW/AbilitySystem/Attributes/TDWHealthSet.h"
#include "TDW/AbilitySystem/Attributes/TDWManaSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWPlayerState)


ATDWPlayerState::ATDWPlayerState(const FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UTDWAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	HealthSet = CreateDefaultSubobject<UTDWHealthSet>(TEXT("HealthSet"));
	ManaSet = CreateDefaultSubobject<UTDWManaSet>(TEXT("ManaSet"));
	CombatSet = CreateDefaultSubobject<UTDWCombatSet>(TEXT("CombatSet"));

	SetNetUpdateFrequency(100.0f);
}

UAbilitySystemComponent* ATDWPlayerState::GetAbilitySystemComponent() const
{
	return GetTDWAbilitySystemComponent();
}

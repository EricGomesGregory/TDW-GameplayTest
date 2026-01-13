// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWCombatComponent.h"

#include "TDW/TDWLogChannels.h"
#include "TDW/AbilitySystem/TDWAbilitySystemComponent.h"
#include "TDW/AbilitySystem/Attributes/TDWCombatSet.h"


// Sets default values for this component's properties
UTDWCombatComponent::UTDWCombatComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	CombatSet = nullptr;
}

void UTDWCombatComponent::InitializeWithAbilitySystem(UTDWAbilitySystemComponent* InASC)
{
	auto* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogTDW, Error, TEXT("CombatComponent: Combat component for owner [%s] has already been initialized with an ability system."), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTDW, Error, TEXT("CombatComponent: Cannot initialize combat component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	CombatSet = AbilitySystemComponent->GetSet<UTDWCombatSet>();
	if (!CombatSet)
	{
		UE_LOG(LogTDW, Error, TEXT("CombatComponent: Cannot initialize combat component for owner [%s] with NULL health set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	CombatSet->OnAttackSpeedChanged.AddUObject(this, &ThisClass::HandleAttackSpeedChange);
	
}

void UTDWCombatComponent::UninitializeFromAbilitySystem()
{
	
}

float UTDWCombatComponent::GetAttackSpeed() const
{
	return (CombatSet ? CombatSet->GetAttackSpeed() : 1.0f);
}

float UTDWCombatComponent::GetBaseDamage() const
{
	return (CombatSet ? CombatSet->GetBaseDamage() : 0.0f);
}

float UTDWCombatComponent::GetBaseHealing() const
{
	return (CombatSet ? CombatSet->GetBaseHeal() : 0.0f);
}

float UTDWCombatComponent::GetBaseManaRegeneration() const
{
	return (CombatSet ? CombatSet->GetBaseManaRegen() : 0.0f);
}

void UTDWCombatComponent::HandleAttackSpeedChange(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue)
{
	
}

void UTDWCombatComponent::HandleBaseDamageChange(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue)
{
	
}

void UTDWCombatComponent::HandleBaseHealingChange(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue)
{
	
}

void UTDWCombatComponent::HandleManaRegenerationChange(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue)
{
	
}





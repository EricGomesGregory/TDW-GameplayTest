// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWHealthComponent.h"

#include "TDW/TDWLogChannels.h"
#include "TDW/AbilitySystem/TDWAbilitySystemComponent.h"
#include "TDW/AbilitySystem/Attributes/TDWHealthSet.h"


UTDWHealthComponent::UTDWHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
}

void UTDWHealthComponent::InitializeWithAbilitySystem(UTDWAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTDW, Error, TEXT("HealthComponent: Cannot initialize mana component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	HealthSet = AbilitySystemComponent->GetSet<UTDWHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogTDW, Error, TEXT("HealthComponent: Cannot initialize mana component for owner [%s] with NULL mana set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	HealthSet->OnHealthChanged.AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);
}

void UTDWHealthComponent::UninitializeFromAbilitySystem()
{
	if (HealthSet)
	{
		HealthSet->OnHealthChanged.RemoveAll(this);
		HealthSet->OnMaxHealthChanged.RemoveAll(this);
		HealthSet->OnOutOfHealth.RemoveAll(this);
	}

	HealthSet = nullptr;
	AbilitySystemComponent = nullptr;
}

float UTDWHealthComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.f);
}

float UTDWHealthComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.f);
}

float UTDWHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();

		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}

	return 0.0f;
}

void UTDWHealthComponent::HandleHealthChanged(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue)
{
	OnHealthChanged.Broadcast(this, OldValue, NewValue, Instigator);
}

void UTDWHealthComponent::HandleMaxHealthChanged(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, Instigator);
}

void UTDWHealthComponent::HandleOutOfHealth(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue)
{
	//@Eric TODO: Trigger event for death 
}



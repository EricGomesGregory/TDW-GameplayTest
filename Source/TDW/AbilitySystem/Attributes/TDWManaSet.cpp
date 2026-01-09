// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWManaSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "TDW/AbilitySystem/TDWAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWManaSet)


UTDWManaSet::UTDWManaSet()
: Mana(100.0f), MaxMana(100.0f)
{
	bOutOfMana = false;
	MaxManaBeforeAttributeChange = 0.0f;
	ManaBeforeAttributeChange = 0.0f;
}

void UTDWManaSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTDWManaSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDWManaSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UTDWManaSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDWManaSet, Mana, OldValue);

	const float CurrentMana = GetMana();
	const float EstimatedMagnitude = CurrentMana - OldValue.GetCurrentValue();
	OnManaChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentMana);

	if (bOutOfMana == false && CurrentMana <= 0.0f)
	{
		OnOutOfMana.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentMana);
	}
}

void UTDWManaSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDWManaSet, Mana, OldValue);
	
	const float Magnitude = GetMaxMana() - OldValue.GetCurrentValue();
	OnMaxManaChanged.Broadcast(nullptr, nullptr, nullptr, Magnitude, OldValue.GetCurrentValue(), GetMaxMana());
}

bool UTDWManaSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if(Super::PreGameplayEffectExecute(Data) == false)
	{
		return false;
	}

	// Save the current health
	ManaBeforeAttributeChange = GetMana();
	MaxManaBeforeAttributeChange = GetMaxMana();

	return true;
}

void UTDWManaSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumMana = 0.0f;
	//@Eric TODO: Handle unlimited health and god-mode cheat states

	const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
	AActor* Instigator = EffectContext.GetOriginalInstigator();
	AActor* Causer = EffectContext.GetEffectCauser();
	
	if (Data.EvaluatedData.Attribute == GetSpendingAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			SetMana(FMath::Clamp(GetMana() - GetSpending(), MinimumMana, GetMaxMana()));
			SetSpending(0.0f);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetRecoveringAttribute())
	{
		// Convert into +Mana and then clamo
		SetMana(FMath::Clamp(GetMana() + GetRecovering(), MinimumMana, GetMaxMana()));
		SetRecovering(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		// Clamp and fall into out of health handling below
		SetMana(FMath::Clamp(GetMana(), MinimumMana, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		// Notify on any requested max health changes
		OnMaxManaChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, MaxManaBeforeAttributeChange, GetMaxMana());
	}

	if (GetMana() != ManaBeforeAttributeChange)
	{
		OnManaChanged.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, ManaBeforeAttributeChange, GetMana());
	}

	if ((GetMana() <= 0.0f) && !bOutOfMana)
	{
		OnOutOfMana.Broadcast(Instigator, Causer, &Data.EffectSpec, Data.EvaluatedData.Magnitude, ManaBeforeAttributeChange, GetMana());
	}

	// Check health again in case an event above changed it.
	bOutOfMana = (GetMana() <= 0.0f);
}

void UTDWManaSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UTDWManaSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	ClampAttribute(Attribute, NewValue);
}

void UTDWManaSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxManaAttribute())
	{
		// Make sure current health is not greater than the new max health.
		if (GetMana() > NewValue)
		{
			auto* TdwASC = GetTDWAbilitySystemComponent();
			check(TdwASC);

			TdwASC->ApplyModToAttribute(GetManaAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (bOutOfMana && (GetMana() > 0.0f))
	{
		bOutOfMana = false;
	}
}

void UTDWManaSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetManaAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}
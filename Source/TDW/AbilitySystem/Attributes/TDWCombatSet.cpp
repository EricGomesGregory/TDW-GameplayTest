// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWCombatSet.h"

#include "Net/UnrealNetwork.h"

UTDWCombatSet::UTDWCombatSet()
: AttackSpeed(1.0f), BaseDamage(10.0f),
BaseHeal(0.f), BaseManaRegen(0.f)
{
}

void UTDWCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTDWCombatSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDWCombatSet, BaseDamage, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UTDWCombatSet, BaseHeal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTDWCombatSet, BaseManaRegen, COND_None, REPNOTIFY_Always);
}

void UTDWCombatSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDWCombatSet, AttackSpeed, OldValue);

	const float CurrentAttackSpeed = GetAttackSpeed();
	const float EstimatedMagnitude = CurrentAttackSpeed - OldValue.GetCurrentValue();
	OnAttackSpeedChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentAttackSpeed);
}

void UTDWCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDWCombatSet, BaseDamage, OldValue);

	const float CurrentBaseDamage = GetBaseDamage();
	const float EstimatedMagnitude = CurrentBaseDamage - OldValue.GetCurrentValue();
	OnBaseDamageChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentBaseDamage);
}

void UTDWCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDWCombatSet, BaseHeal, OldValue);

	const float CurrentBaseHeal = GetBaseDamage();
	const float EstimatedMagnitude = CurrentBaseHeal - OldValue.GetCurrentValue();
	OnBaseHealChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentBaseHeal);
}

void UTDWCombatSet::OnRep_BaseManaRegen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDWCombatSet, BaseManaRegen, OldValue);

	const float CurrentBaseManaRegen = GetBaseDamage();
	const float EstimatedMagnitude = CurrentBaseManaRegen - OldValue.GetCurrentValue();
	OnBaseManaRegenChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentBaseManaRegen);
}

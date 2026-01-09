// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWCombatSet.h"

#include "Net/UnrealNetwork.h"

UTDWCombatSet::UTDWCombatSet()
: AttackSpeed(1.0f)
{
		
}

void UTDWCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTDWCombatSet, AttackSpeed, COND_None, REPNOTIFY_Always);
}

void UTDWCombatSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTDWCombatSet, AttackSpeed, OldValue);

	const float CurrentAttackSpeed = GetAttackSpeed();
	const float EstimatedMagnitude = CurrentAttackSpeed - OldValue.GetCurrentValue();
	OnAttackSpeedChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentAttackSpeed);
}

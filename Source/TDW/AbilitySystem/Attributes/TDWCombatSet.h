// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDWAttributeSet.h"
#include "TDWCombatSet.generated.h"

/**
 * 
 */
UCLASS()
class TDW_API UTDWCombatSet : public UTDWAttributeSet
{
	GENERATED_BODY()

public:

	UTDWCombatSet();

	ATTRIBUTE_ACCESSORS(UTDWCombatSet, AttackSpeed);

	mutable FTDWAttributeEvent OnAttackSpeedChanged;

protected:

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue);

private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AttackSpeed, Category="TDW|Combat", Meta=(AllowPrivateAccess = true))
	FGameplayAttributeData AttackSpeed;
};

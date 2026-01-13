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
	ATTRIBUTE_ACCESSORS(UTDWCombatSet, BaseDamage);

	ATTRIBUTE_ACCESSORS(UTDWCombatSet, BaseHeal);
	ATTRIBUTE_ACCESSORS(UTDWCombatSet, BaseManaRegen);

	/**  */
	mutable FTDWAttributeEvent OnAttackSpeedChanged;

	/**  */
	mutable FTDWAttributeEvent OnBaseDamageChanged;

	/**  */
	mutable FTDWAttributeEvent OnBaseHealChanged;

	/**  */
	mutable FTDWAttributeEvent OnBaseManaRegenChanged;

protected:

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseHeal(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BaseManaRegen(const FGameplayAttributeData& OldValue);

private:
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AttackSpeed, Category="TDW|Combat", Meta=(AllowPrivateAccess = true))
	FGameplayAttributeData AttackSpeed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BaseDamage, Category="TDW|Combat", Meta=(AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BaseHeal, Category="TDW|Combat", Meta=(AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BaseManaRegen, Category="TDW|Combat", Meta=(AllowPrivateAccess = true))
	FGameplayAttributeData BaseManaRegen;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDWAttributeSet.h"
#include "TDWManaSet.generated.h"

/**
 * 
 */
UCLASS()
class TDW_API UTDWManaSet : public UTDWAttributeSet
{
	GENERATED_BODY()

public:

	UTDWManaSet();

	ATTRIBUTE_ACCESSORS(UTDWManaSet, Mana);
	ATTRIBUTE_ACCESSORS(UTDWManaSet, MaxMana);
	ATTRIBUTE_ACCESSORS(UTDWManaSet, Recovering);
	ATTRIBUTE_ACCESSORS(UTDWManaSet, Spending);

	// Delegate when health changes due to damage/healing, some information may be missing on the client
	mutable FTDWAttributeEvent OnManaChanged;

	// Delegate when max health changes
	mutable FTDWAttributeEvent OnMaxManaChanged;

	// Delegate to broadcast when the health attribute reaches zero
	mutable FTDWAttributeEvent OnOutOfMana;

	
protected:

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	
private:
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="TDW|Mana", meta=(HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="TDW|Mana", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData MaxMana;

	// Used to track when the health reaches 0.
	bool bOutOfMana;

	// Store the health before any changes 
	float MaxManaBeforeAttributeChange;
	float ManaBeforeAttributeChange;
	
	// -------------------- //
	// Meta Attributes      //
	// -------------------- //

	UPROPERTY(BlueprintReadOnly, Category="TDW|Mana", Meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Recovering;

	UPROPERTY(BlueprintReadOnly, Category="TDW|Mana", Meta=(HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Spending;
};

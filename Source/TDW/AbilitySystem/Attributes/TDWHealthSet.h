// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDWAttributeSet.h"
#include "TDWHealthSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TDW_API UTDWHealthSet : public UTDWAttributeSet
{
	GENERATED_BODY()

public:
	UTDWHealthSet();

	ATTRIBUTE_ACCESSORS(UTDWHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UTDWHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UTDWHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UTDWHealthSet, Damage);

	// Delegate when health changes due to damage/healing, some information may be missing on the client
	mutable FTDWAttributeEvent OnHealthChanged;

	// Delegate when max health changes
	mutable FTDWAttributeEvent OnMaxHealthChanged;

	// Delegate to broadcast when the health attribute reaches zero
	mutable FTDWAttributeEvent OnOutOfHealth;
	
protected:

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	
private:
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="TDW|Health", meta=(HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="TDW|Health", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// Used to track when the health reaches 0.
	bool bOutOfHealth;

	// Store the health before any changes 
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;
	
	// -------------------- //
	// Meta Attributes      //
	// -------------------- //

	UPROPERTY(BlueprintReadOnly, Category="TDW|Health", Meta=(AllowPrivateAccess = true))
	FGameplayAttributeData Healing;

	UPROPERTY(BlueprintReadOnly, Category="TDW|Health", Meta=(HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Damage;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/ActorComponent.h"
#include "TDWCombatComponent.generated.h"


class UTDWCombatSet;
class UTDWAbilitySystemComponent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDW_API UTDWCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTDWCombatComponent();

	UFUNCTION(BlueprintPure, Category="TDW|Combat")
	static UTDWCombatComponent* FindCombatComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UTDWCombatComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "TDW|Combat")
	void InitializeWithAbilitySystem(UTDWAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "TDW|Combat")
	void UninitializeFromAbilitySystem();

	// Returns the current attack speed value.
	UFUNCTION(BlueprintCallable, Category = "TDW|Combat")
	float GetAttackSpeed() const;

	// Returns the current base damage value.
	UFUNCTION(BlueprintCallable, Category = "TDW|Combat")
	float GetBaseDamage() const;

	// Returns the current base heal value.
	UFUNCTION(BlueprintCallable, Category = "TDW|Combat")
	float GetBaseHealing() const;

	// Returns the current attack speed value.
	UFUNCTION(BlueprintCallable, Category = "TDW|Combat")
	float GetBaseManaRegeneration() const;
	
protected:

	virtual void HandleAttackSpeedChange(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);
	virtual void HandleBaseDamageChange(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);
	virtual void HandleBaseHealingChange(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);
	virtual void HandleManaRegenerationChange(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);
	
protected:
	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UTDWAbilitySystemComponent> AbilitySystemComponent;

	// Combat set used by this component.
	UPROPERTY()
	TObjectPtr<const UTDWCombatSet> CombatSet;
};

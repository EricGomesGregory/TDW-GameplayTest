// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/ActorComponent.h"
#include "TDWManaComponent.generated.h"


class UTDWManaSet;
class UTDWAbilitySystemComponent;


UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class TDW_API UTDWManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTDWManaComponent();

	UFUNCTION(BlueprintPure, Category="TDW|ManaComponent")
	static UTDWManaComponent* FindManaComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UTDWManaComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "TDW|Mana")
	void InitializeWithAbilitySystem(UTDWAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "TDW|Mana")
	void UninitializeFromAbilitySystem();
	
	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "TDW|Mana")
	float GetMana() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "TDW|Mana")
	float GetMaxMana() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "TDW|Mana")
	float GetManaNormalized() const;	
	
protected:

	virtual void HandleManaChanged(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);
	virtual void HandleMaxManaChanged(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);
	virtual void HandleOutOfMana(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);

	
protected:

	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UTDWAbilitySystemComponent> AbilitySystemComponent;

	// Mana set used by this component.
	UPROPERTY()
	TObjectPtr<const UTDWManaSet> ManaSet;
};

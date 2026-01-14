// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/ActorComponent.h"
#include "TDWManaComponent.generated.h"

class UTDWManaSet;
class UTDWAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FTDWMana_AttributeChanged, UTDWManaComponent*, ManaComponent, float, OldValue, float, NewValue, AActor*, Instigator);


UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class TDW_API UTDWManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**  */
	UTDWManaComponent();

	UFUNCTION(BlueprintPure, Category="TDW|Mana")
	static UTDWManaComponent* FindManaComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UTDWManaComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "TDW|Mana")
	void InitializeWithAbilitySystem(UTDWAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "TDW|Mana")
	void UninitializeFromAbilitySystem();
	
	// Returns the current mana value.
	UFUNCTION(BlueprintCallable, Category = "TDW|Mana")
	float GetMana() const;

	// Returns the current maximum mana value.
	UFUNCTION(BlueprintCallable, Category = "TDW|Mana")
	float GetMaxMana() const;

	// Returns the current mana in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "TDW|Mana")
	float GetManaNormalized() const;	
	
protected:

	virtual void HandleManaChanged(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);
	virtual void HandleMaxManaChanged(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);
	virtual void HandleOutOfMana(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);

public:

	// Delegate fired when the mana value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FTDWMana_AttributeChanged OnManaChanged;

	// Delegate fired when the max mana value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FTDWMana_AttributeChanged OnMaxManaChanged;
	
protected:

	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UTDWAbilitySystemComponent> AbilitySystemComponent;

	// Mana set used by this component.
	UPROPERTY()
	TObjectPtr<const UTDWManaSet> ManaSet;
};

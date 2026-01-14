// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/ActorComponent.h"
#include "TDWHealthComponent.generated.h"

class UTDWHealthSet;
class UTDWAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FTDWHealth_AttributeChanged, UTDWHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TDW_API UTDWHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**  */
	UTDWHealthComponent();

	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "TDW|Health")
	static UTDWHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UTDWHealthComponent>() : nullptr); }

	// Initialize the component using an ability system component.
	UFUNCTION(BlueprintCallable, Category = "TDW|Health")
	void InitializeWithAbilitySystem(UTDWAbilitySystemComponent* InASC);

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "TDW|Health")
	void UninitializeFromAbilitySystem();

	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "TDW|Health")
	float GetHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "TDW|Health")
	float GetMaxHealth() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "TDW|Health")
	float GetHealthNormalized() const;

protected:

	virtual void HandleHealthChanged(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);
	virtual void HandleMaxHealthChanged(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);
	virtual void HandleOutOfHealth(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue);

	
public:

	// Delegate fired when the health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FTDWHealth_AttributeChanged OnHealthChanged;

	// Delegate fired when the max health value has changed. This is called on the client but the instigator may not be valid
	UPROPERTY(BlueprintAssignable)
	FTDWHealth_AttributeChanged OnMaxHealthChanged;
	
protected:

	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UTDWAbilitySystemComponent> AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UTDWHealthSet> HealthSet;
};

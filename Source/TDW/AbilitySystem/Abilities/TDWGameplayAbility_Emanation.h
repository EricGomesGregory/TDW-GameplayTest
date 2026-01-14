// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TDWGameplayAbility.h"
#include "TDWGameplayAbility_Emanation.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class TDW_API UTDWGameplayAbility_Emanation : public UTDWGameplayAbility
{
	GENERATED_BODY()

public:
	UTDWGameplayAbility_Emanation();

protected:

	UFUNCTION(BlueprintCallable, Category="TDW|Ability")
	bool GetActorsInEmanation(TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable, Category="TDW|Ability")
	void ApplyEffectsToTaget(UAbilitySystemComponent* TargetASC);

	UFUNCTION(BlueprintCallable, Category="TDW|Ability")
	FVector GetEmanationOrigin() const;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="TDW|Ability")
	float EmanationRadius;

	UPROPERTY(EditDefaultsOnly, Category="TDW|Ability")
	TArray<TSubclassOf<UGameplayEffect>> EmanationEffects;
};

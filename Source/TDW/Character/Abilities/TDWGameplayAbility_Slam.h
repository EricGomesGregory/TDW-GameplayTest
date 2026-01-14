// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDW/AbilitySystem/Abilities/TDWGameplayAbility_Emanation.h"
#include "TDWGameplayAbility_Slam.generated.h"

/**
 * 
 */
UCLASS()
class TDW_API UTDWGameplayAbility_Slam : public UTDWGameplayAbility_Emanation
{
	GENERATED_BODY()

public:
	UTDWGameplayAbility_Slam();

protected:
	UFUNCTION(BlueprintCallable, Category="TDW|Ability")
	void ApplyKnockback(AActor* TargetActor, const FVector& SourceLocation, /*float HorizontalStrength, float VerticalStrength,*/ bool bOverrideVelocity);
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TDW|Ability")
	float HorizontalKnockbackForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TDW|Ability")
	float VerticalKnockbackForce;
};

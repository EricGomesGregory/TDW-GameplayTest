// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "TDWDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class TDW_API UTDWDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	/**  */
	UTDWDamageExecution();

protected:
	/**  */
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

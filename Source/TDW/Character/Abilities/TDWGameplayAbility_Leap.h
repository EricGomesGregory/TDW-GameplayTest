// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDW/AbilitySystem/Abilities/TDWGameplayAbility.h"
#include "TDWGameplayAbility_Leap.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class TDW_API UTDWGameplayAbility_Leap : public UTDWGameplayAbility
{
	GENERATED_BODY()

public:
	
	UTDWGameplayAbility_Leap();

protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnLanded();
	
	FVector ResolveTargetLocation(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* TriggerEventData) const;
	float ResolveJumpHeight(const float Distance) const;
	
	bool FindGroundAtLocation(const FVector& InLocation, FVector& OutGroundLocation, float TraceUp = 500.f, float TraceDown = 2000.f) const;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="TDW|Ability")
	float MaxLeapDistance;

	UPROPERTY(EditDefaultsOnly, Category="TDW|Ability")
	float MinLeapDistance;

	UPROPERTY(EditDefaultsOnly, Category="TDW|Ability")
	TObjectPtr<UCurveFloat> LeapHeightCurve;

	UPROPERTY(EditDefaultsOnly, Category="TDW|Ability")
	float LeapDuration;

	UPROPERTY(EditDefaultsOnly, Category="TDW|Ability")
	TArray<TSubclassOf<UTDWGameplayAbility>> LandingAbilities;

private:

	void GiveChildAbilities(UTDWAbilitySystemComponent* InASC, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	void TakeChildAbilities(UTDWAbilitySystemComponent* InASC);
	
private:
	/**  */
	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> ChildAbilitySpecHandles;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TDWGameplayAbility.generated.h"

class ATDWPlayerController;
class ATDWCharacter;
class UTDWAbilitySystemComponent;
/**
 * 
 */
UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnSpawn,
};

/**
 * 
 */
UCLASS(Abstract, HideCategories=(Input))
class TDW_API UTDWGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UTDWGameplayAbility();

	UFUNCTION(BlueprintCallable, Category = "TDW|Ability")
	UTDWAbilitySystemComponent* GetTDWAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "TDW|Ability")
	ATDWPlayerController* GetTDWPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "TDW|Ability")
	AController* GetControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "TDW|Ability")
	ATDWCharacter* GetTDWCharacterFromActorInfo() const;
	
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;
	
	EAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

protected:

	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TDW|Ability Activation")
	EAbilityActivationPolicy ActivationPolicy;
};

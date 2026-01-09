// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "TDWPlayerState.generated.h"

class UTDWAbilitySystemComponent;
class UTDWCombatSet;
class UTDWHealthSet;
class UTDWManaSet;


/**
 * 
 */
UCLASS(Config = Game)
class TDW_API ATDWPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ATDWPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "TDW|PlayerState")
	UTDWAbilitySystemComponent* GetTDWAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	
private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "TDW|PlayerState")
	TObjectPtr<UTDWAbilitySystemComponent> AbilitySystemComponent;

	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const UTDWHealthSet> HealthSet;

	// Mana attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const UTDWManaSet> ManaSet;
	
	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const UTDWCombatSet> CombatSet;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TDWNonPlayerCharacter.generated.h"

class UTDWCombatSet;
class UTDWHealthSet;
class UTDWHealthComponent;
class UTDWCombatComponent;
class UTDWAbilitySystemComponent;


UCLASS()
class TDW_API ATDWNonPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATDWNonPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "TDW|Character")
	FORCEINLINE UTDWAbilitySystemComponent* GetTDWAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	//~ Begin APawn Interface.
	virtual void PostInitializeComponents() override;
	virtual  void PostUnregisterAllComponents() override;
	//~ End APawn Interface
	
protected:
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDW|Character")
	TObjectPtr<UTDWAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TDW|Character")
	TObjectPtr<UTDWHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="TDW|Character")
	TObjectPtr<UTDWCombatComponent> CombatComponent;
	
private:
	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<UTDWHealthSet> HealthSet;

	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<UTDWCombatSet> CombatSet;
};

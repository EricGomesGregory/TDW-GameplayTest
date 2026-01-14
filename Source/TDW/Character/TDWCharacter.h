// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "TDWCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTDWHealthComponent;
class UTDWCombatComponent;
class UTDWManaComponent;
class UTDWCombatSet;
class UTDWManaSet;
class UTDWHealthSet;
class UTDWAbilitySystemComponent;
class UTDWAbilitySet;


UCLASS(Blueprintable)
class ATDWCharacter : public ACharacter, public  IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATDWCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintPure, Category = "TDW|Character")
	FORCEINLINE UTDWAbilitySystemComponent* GetTDWAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual  UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintPure, Category = "TDW|Character")
	FORCEINLINE UTDWCombatComponent* GetCombatComponent() const { return CombatComponent; }

	UFUNCTION(BlueprintPure, Category = "TDW|Character")
	FORCEINLINE UTDWHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintPure, Category = "TDW|Character")
	FORCEINLINE UTDWManaComponent* GetManaComponent() const { return ManaComponent; }
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	
	virtual void OnRep_PlayerState() override;
	
protected:

	void InitializeAbilityActorInfo();
	void SetupAbilities();
	
protected:
	UPROPERTY(EditAnywhere, Category="TDW|Character")
	TObjectPtr<UTDWAbilitySet> AbilitySet;

	UPROPERTY()
	TObjectPtr<UTDWAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, Category="TDW|Character")
	TObjectPtr<UTDWCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, Category="TDW|Character")
	TObjectPtr<UTDWHealthComponent> HealthComponent;
	
	UPROPERTY(VisibleAnywhere, Category="TDW|Character")
	TObjectPtr<UTDWManaComponent> ManaComponent;
	
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
};


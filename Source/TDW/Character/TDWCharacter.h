// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TDWCharacter.generated.h"

class UTDWCombatSet;
class UTDWManaSet;
class UTDWHealthSet;
class UTDWAbilitySystemComponent;
class UTDWAbilitySet;


UCLASS(Blueprintable)
class ATDWCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDWCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
protected:

	void InitializeAbilityActorInfo();
	void SetupAbilities();
	
protected:
	UPROPERTY(EditAnywhere, Category="TDW|Ability System")
	TObjectPtr<UTDWAbilitySet> AbilitySet;

	UPROPERTY()
	TObjectPtr<UTDWAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UTDWHealthSet> HealthSet;

	UPROPERTY()
	TObjectPtr<const UTDWManaSet> ManaSet;
	
	UPROPERTY()
	TObjectPtr<const UTDWCombatSet> CombatSet;
	
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};


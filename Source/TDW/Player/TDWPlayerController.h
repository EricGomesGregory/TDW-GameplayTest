// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "TDWPlayerController.generated.h"

class ATDWPlayerState;
class UTDWAbilitySystemComponent;
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UTDWInputConfig;


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class ATDWPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATDWPlayerController();

	UFUNCTION(BlueprintCallable, Category = "TDW|PlayerController")
	ATDWPlayerState* GetTDWPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "TDW|PlayerController")
	UTDWAbilitySystemComponent* GetTDWAbilitySystemComponent() const;
	
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
	/** Input handlers for SetDestination action. */
	void Input_StopMovement();
	void Input_SetDestinationTriggered();
	void Input_SetDestinationReleased();
	//void OnTouchTriggered();
	//void OnTouchReleased();

protected:

	UPROPERTY(EditAnywhere, Category=Input)
	TObjectPtr<UTDWInputConfig> InputConfig;
	
private:
	FVector CachedDestination;

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};



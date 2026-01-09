// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TDWInputConfig.generated.h"


class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FInputMappingContextAndPriority
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> InputMapping = nullptr;

	// Higher priority input mappings will be prioritized over mappings with a lower priority.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Priority = 0;

	// If true, then this mapping context will be registered with the settings when this game feature action is registered.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bRegisterWithSettings = true;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTDWInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories = "Input"))
	FGameplayTag InputTag;
};


/**
 * 
 */
UCLASS(BlueprintType, Const)
class TDW_API UTDWInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UTDWInputConfig();

	UFUNCTION(BlueprintCallable, Category = "TDW|Input")
	const UInputAction* FindNativeInputActionFromTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "TDW|Input")
	const UInputAction* FindAbilityInputActionFromTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty = "InputAction"))
	TArray<FTDWInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty = "InputAction"))
	TArray<FTDWInputAction> AbilityInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInputMappingContextAndPriority> DefaultInputMappings;
};

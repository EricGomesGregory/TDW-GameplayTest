// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TDWInputConfig.generated.h"


class UInputAction;

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
};

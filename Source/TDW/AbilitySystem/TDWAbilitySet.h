// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "TDWAbilitySet.generated.h"

class UTDWAbilitySystemComponent;
class UTDWGameplayAbility;
class UGameplayEffect;
class UAttributeSet;


/**
 * 
 */
UENUM()
enum class EAbilityLevelSource
{
	FromAvatarActor, // Ability level depends on avatar actor
	//@Eric TODO: Add additional sources based on project necessity such as:
	// FromEquipment,
	Custom, // Has a custom value
};

/**
 * 
 */
UENUM()
enum class EEffectLevelSource
{
	FromAvatarActor, // Effect level depends on avatar actor
	//@Eric TODO: Add additional sources based on project necessity such as: 
	// FromEquipment,
	Custom, // Has a custom value
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTDWAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTDWGameplayAbility> Ability = nullptr;

	// Source of ability level to grant.
	UPROPERTY(EditDefaultsOnly)
	EAbilityLevelSource LevelSource = EAbilityLevelSource::Custom;
	
	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly, meta=(EditConditionHides, EditCondition = "LevelSource==EAbilityLevelSource::Custom"))
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "Input"))
	FGameplayTag InputTag;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTDWAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly)
	EEffectLevelSource LevelSource = EEffectLevelSource::Custom;
	
	// Level of gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly, meta=(EditConditionHides, EditCondition = "LevelSource==EEffectLevelSource::Custom"))
	float EffectLevel = 1.0f;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTDWAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTDWAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(UTDWAbilitySystemComponent* TDWASC);

protected:
	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Pointers to the granted attribute sets
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};



/**
 * 
 */
UCLASS(BlueprintType, Const)
class TDW_API UTDWAbilitySet : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UTDWAbilitySet();
	
	// Grants the ability set to the specified ability system component.
	// The returned handles can be used later to take away anything that was granted.
	void GiveToAbilitySystem(UTDWAbilitySystemComponent* TDWASC, FTDWAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FTDWAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta=(TitleProperty=GameplayEffect))
	TArray<FTDWAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// Attribute sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta=(TitleProperty=AttributeSet))
	TArray<FTDWAbilitySet_AttributeSet> GrantedAttributes;
};

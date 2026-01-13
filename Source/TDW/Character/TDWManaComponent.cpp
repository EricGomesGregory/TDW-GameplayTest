// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWManaComponent.h"

#include "TDW/TDWGameplayTags.h"
#include "TDW/TDWLogChannels.h"
#include "TDW/AbilitySystem/TDWAbilitySystemComponent.h"
#include "TDW/AbilitySystem/Attributes/TDWManaSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWManaComponent)

UTDWManaComponent::UTDWManaComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	ManaSet = nullptr;
}

void UTDWManaComponent::InitializeWithAbilitySystem(UTDWAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTDW, Error, TEXT("ManaComponent: Cannot initialize mana component for owner [%s] with NULL ability system."), *GetNameSafe(Owner));
		return;
	}

	ManaSet = AbilitySystemComponent->GetSet<UTDWManaSet>();
	if (!ManaSet)
	{
		UE_LOG(LogTDW, Error, TEXT("ManaComponent: Cannot initialize mana component for owner [%s] with NULL mana set on the ability system."), *GetNameSafe(Owner));
		return;
	}

	ManaSet->OnManaChanged.AddUObject(this, &ThisClass::HandleManaChanged);
	ManaSet->OnMaxManaChanged.AddUObject(this, &ThisClass::HandleMaxManaChanged);
	ManaSet->OnOutOfMana.AddUObject(this, &ThisClass::HandleOutOfMana);
}

void UTDWManaComponent::UninitializeFromAbilitySystem()
{
	
}

float UTDWManaComponent::GetMana() const
{
	return (ManaSet ? ManaSet->GetMana() : 0.0f);
}

float UTDWManaComponent::GetMaxMana() const
{
	return (ManaSet ? ManaSet->GetMaxMana() : 0.0f);
}

float UTDWManaComponent::GetManaNormalized() const
{
	if (ManaSet)
	{
		const float Mana = ManaSet->GetMana();
		const float MaxMana = ManaSet->GetMaxMana();

		return ((MaxMana > 0.0f) ? (Mana / MaxMana) : 0.0f);
	}

	return 0.0f;
}

void UTDWManaComponent::HandleManaChanged(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue)
{
	//@Eric TODO: Hacky solution, update in the future 
	if (AbilitySystemComponent && OldValue == 0.0f && NewValue != 0.0f)
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(TDWGameplayTags::GameplayEvent_OutOfMana))
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(TDWGameplayTags::GameplayEvent_OutOfMana);
		}
	}
}

void UTDWManaComponent::HandleMaxManaChanged(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue)
{
	
}

void UTDWManaComponent::HandleOutOfMana(AActor* Instigator, AActor* Causer, const FGameplayEffectSpec* EffectSpec, float Magnitude, float OldValue, float NewValue)
{
#if WITH_SERVER_CODE
	
	if (AbilitySystemComponent && EffectSpec)
	{
		FGameplayEventData Payload;
		Payload.EventTag = TDWGameplayTags::GameplayEvent_OutOfMana;
		Payload.Instigator = Instigator;
		Payload.Target = AbilitySystemComponent->GetAvatarActor();
		Payload.OptionalObject = EffectSpec->Def;
		Payload.ContextHandle = EffectSpec->GetEffectContext();
		Payload.InstigatorTags = *EffectSpec->CapturedSourceTags.GetAggregatedTags();
		Payload.TargetTags = *EffectSpec->CapturedTargetTags.GetAggregatedTags();
		Payload.EventMagnitude = Magnitude;

		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
		AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
	}

	//@Eric TODO: Hacky solution, update in the future 
	AbilitySystemComponent->AddLooseGameplayTag(TDWGameplayTags::GameplayEvent_OutOfMana);
	
	//@Eric TODO: Maybe implement additional messages using GameplayMessageSubsystem
	
#endif // WITH_SERVER_CODE
}




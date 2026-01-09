// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWAbilitySet.h"
#include "TDW/AbilitySystem/Abilities/TDWGameplayAbility.h"
#include "TDWAbilitySystemComponent.h"
#include "TDW/TDWLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWAbilitySet)


void FTDWAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FTDWAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FTDWAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FTDWAbilitySet_GrantedHandles::TakeFromAbilitySystem(UTDWAbilitySystemComponent* TDWASC)
{
	check(TDWASC);

	if (!TDWASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			TDWASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			TDWASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		TDWASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UTDWAbilitySet::UTDWAbilitySet()
: Super()
{
}

void UTDWAbilitySet::GiveToAbilitySystem(UTDWAbilitySystemComponent* TDWASC, FTDWAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(TDWASC);

	if (!TDWASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FTDWAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTDWAbilitySystem, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UTDWGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UTDWGameplayAbility>();

		int AbilityLevel = 1;
		switch (AbilityToGrant.LevelSource)
		{
		case EAbilityLevelSource::FromAvatarActor:
			AbilityLevel = TDWASC->GetAvatarActorLevel();
			break;
		case EAbilityLevelSource::Custom:
			AbilityLevel = AbilityToGrant.AbilityLevel;
			break;
		}
		
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = TDWASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FTDWAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTDWAbilitySystem, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		float EffectLevel = 1.0f;
		switch (EffectToGrant.LevelSource)
		{
			case EEffectLevelSource::FromAvatarActor:
			EffectLevel = TDWASC->GetAvatarActorLevel();
			break;
			case EEffectLevelSource::Custom:
			EffectLevel = EffectToGrant.EffectLevel;
			break;
		}
		
		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = TDWASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectLevel, TDWASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FTDWAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogTDWAbilitySystem, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(TDWASC->GetOwner(), SetToGrant.AttributeSet);
		TDWASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
}
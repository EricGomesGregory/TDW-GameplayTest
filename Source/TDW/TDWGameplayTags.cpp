// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWGameplayTags.h"

namespace TDWGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SetDestination, "Input.SetDestination", "Input tag for setting destination");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_SetTarget, "Input.SetTarget", "Input tag for setting a target location or enemy");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Input_Skill1, "Input.Skill1", "Input tag for the 1st active skill");

	UE_DEFINE_GAMEPLAY_TAG(Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_OutOfMana, "GameplayEvent.OutOfMana", "Event that fires on out of mana. This event only fires on the server.");
}

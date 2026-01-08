// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWAttributeSet.h"
#include "TDW/AbilitySystem/TDWAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWAttributeSet)


UTDWAttributeSet::UTDWAttributeSet()
{
}

UWorld* UTDWAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UTDWAbilitySystemComponent* UTDWAttributeSet::GetTDWAbilitySystemComponent() const
{
	return Cast<UTDWAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}

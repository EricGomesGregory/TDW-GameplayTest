// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWGameplayAbility_Emanation.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/OverlapResult.h"


UTDWGameplayAbility_Emanation::UTDWGameplayAbility_Emanation()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	ActivationPolicy = EAbilityActivationPolicy::OnSpawn;
	
	EmanationRadius = 250.0f;

	bHasKnockback = false;
	KnockbackForce = 50.0f;
}

void UTDWGameplayAbility_Emanation::ActorEmanationDoOnce()
{
	if (EmanationEffects.Num() == 0) { return; }

	TArray<FOverlapResult> Overlaps;
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(EmanationRadius);

	const auto* AvatarActor = GetAvatarActorFromActorInfo();
	check(AvatarActor);

	const FVector TargetLocation = AvatarActor->GetActorLocation();
	GetWorld()->OverlapMultiByChannel(Overlaps, TargetLocation, FQuat::Identity, ECC_Pawn, Sphere);
#if !UE_BUILD_SHIPPING
	DrawDebugSphere(GetWorld(), TargetLocation, EmanationRadius, 12, FColor::Red, false, 1, 0, 1);
#endif // UE_BUILD_SHIPPING
	
	auto* ASC = GetAbilitySystemComponentFromActorInfo();

	for (const FOverlapResult& Result : Overlaps)
	{
		const auto* Target = Result.GetActor();
		if (!Target || Target == GetAvatarActorFromActorInfo()) { continue; }

		if (auto* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target))
		{
			ApplyEffectsToTaget(ASC, TargetASC);
		}
	}
}

void UTDWGameplayAbility_Emanation::ActorEmanationStart()
{
	if (EmanationEffects.Num() == 0) { return; }

	//@Eric TODO: Implement in the future for aura-like abilities
}

void UTDWGameplayAbility_Emanation::ActorEmanationStop()
{
	if (EmanationEffects.Num() == 0) { return; }

	//@Eric TODO: Implement in the future for aura-like abilities 
}

void UTDWGameplayAbility_Emanation::ApplyEffectsToTaget(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC)
{
	for (const auto& Effect : EmanationEffects)
	{
		FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(Effect, GetAbilityLevel());

		SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
	}
}


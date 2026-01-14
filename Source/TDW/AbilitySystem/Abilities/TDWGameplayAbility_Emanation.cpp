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
}

bool UTDWGameplayAbility_Emanation::GetActorsInEmanation(TArray<AActor*>& OutActors)
{
	TArray<FOverlapResult> Overlaps;
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(EmanationRadius);

	const auto* AvatarActor = GetAvatarActorFromActorInfo();
	check(AvatarActor);

	const FVector TargetLocation = AvatarActor->GetActorLocation();
	GetWorld()->OverlapMultiByChannel(Overlaps, TargetLocation, FQuat::Identity, ECC_Pawn, Sphere);
#if !UE_BUILD_SHIPPING
	DrawDebugSphere(GetWorld(), TargetLocation, EmanationRadius, 12, FColor::Red, false, 1, 0, 1);
#endif // UE_BUILD_SHIPPING

	for (const FOverlapResult& Result : Overlaps)
	{
		auto* Target = Result.GetActor();
		if (!Target || Target == GetAvatarActorFromActorInfo()) { continue; }

		OutActors.Add(Target);
	}

	return OutActors.IsEmpty();
}

void UTDWGameplayAbility_Emanation::ApplyEffectsToTaget(UAbilitySystemComponent* TargetASC)
{
	if (!TargetASC) { return; }
	
	auto* ASC = GetAbilitySystemComponentFromActorInfo();
	check(ASC);

	for (const auto& Effect : EmanationEffects)
	{
		FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(Effect, GetAbilityLevel());

		ASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
	}
}

FVector UTDWGameplayAbility_Emanation::GetEmanationOrigin() const
{
	return GetAvatarActorFromActorInfo()->GetActorLocation();
}



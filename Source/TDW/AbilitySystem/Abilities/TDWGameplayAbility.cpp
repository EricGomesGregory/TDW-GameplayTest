// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWGameplayAbility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWGameplayAbility)

UTDWGameplayAbility::UTDWGameplayAbility()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

	ActivationPolicy = EAbilityActivationPolicy::OnInputTriggered;
}

void UTDWGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	//@Eric TODO: Handle passive ability 
}

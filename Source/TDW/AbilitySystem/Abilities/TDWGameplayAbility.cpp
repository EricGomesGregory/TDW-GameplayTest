// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWGameplayAbility.h"

#include "TDW/AbilitySystem/TDWAbilitySystemComponent.h"
#include "TDW/Character/TDWCharacter.h"
#include "TDW/Player/TDWPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWGameplayAbility)


UTDWGameplayAbility::UTDWGameplayAbility()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

	ActivationPolicy = EAbilityActivationPolicy::OnInputTriggered;
}

UTDWAbilitySystemComponent* UTDWGameplayAbility::GetTDWAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UTDWAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
}

ATDWPlayerController* UTDWGameplayAbility::GetTDWPlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ATDWPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

AController* UTDWGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (auto* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		auto* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (auto* C = Cast<AController>(TestActor))
			{
				return C;
			}

			if (const auto* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

ATDWCharacter* UTDWGameplayAbility::GetTDWCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ATDWCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

ACharacter* UTDWGameplayAbility::GetCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}

void UTDWGameplayAbility::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const
{
	// Try to activate if activation policy is on spawn.
	if (ActorInfo && !Spec.IsActive() && (ActivationPolicy == EAbilityActivationPolicy::OnSpawn))
	{
		auto* ASC = ActorInfo->AbilitySystemComponent.Get();
		const auto* AvatarActor = ActorInfo->AvatarActor.Get();

		// If avatar actor is torn off or about to die, don't try to activate until we get the new one.
		if (ASC && AvatarActor && !AvatarActor->GetTearOff() && (AvatarActor->GetLifeSpan() <= 0.0f))
		{
			const bool bIsLocalExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalOnly);
			const bool bIsServerExecution = (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerOnly) || (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::ServerInitiated);

			const bool bClientShouldActivate = ActorInfo->IsLocallyControlled() && bIsLocalExecution;
			const bool bServerShouldActivate = ActorInfo->IsNetAuthority() && bIsServerExecution;

			if (bClientShouldActivate || bServerShouldActivate)
			{
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
	}
}

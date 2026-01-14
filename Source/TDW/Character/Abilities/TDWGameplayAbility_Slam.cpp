// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWGameplayAbility_Slam.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UTDWGameplayAbility_Slam::UTDWGameplayAbility_Slam()
{
	HorizontalKnockbackForce	= 500.0;
	VerticalKnockbackForce		= 150.f;
}

void UTDWGameplayAbility_Slam::ApplyKnockback(AActor* TargetActor, const FVector& SourceLocation, /*float HorizontalStrength, float VerticalStrength,*/ bool bOverrideVelocity)
{
	check(TargetActor);

	FVector KnockbackDir = TargetActor->GetActorLocation() - SourceLocation;
	KnockbackDir.Z = 0.f;
	KnockbackDir.Normalize();

	FVector KnockbackVelocity = KnockbackDir * HorizontalKnockbackForce;
	KnockbackVelocity.Z = VerticalKnockbackForce;

	// Prefer CharacterMovement if possible
	if (ACharacter* Character = Cast<ACharacter>(TargetActor))
	{
		if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
		{
			if (Character->HasAuthority())
			{
				MoveComp->StopMovementImmediately();

				if (bOverrideVelocity)
				{
					MoveComp->Velocity = KnockbackVelocity;
				}
				else
				{
					MoveComp->Velocity += KnockbackVelocity;
				}

				MoveComp->SetMovementMode(MOVE_Falling);
			}

			return;
		}
	}

	// Physics fallback (ragdolls, destructibles, props)
	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(TargetActor->GetRootComponent()))
	{
		if (PrimComp->IsSimulatingPhysics())
		{
			PrimComp->AddImpulse(KnockbackVelocity, NAME_None, true);
		}
	}
}

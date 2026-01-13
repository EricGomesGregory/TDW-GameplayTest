// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWGameplayAbility_Leap.h"

#include "AbilitySystemComponent.h"
#include "NavigationSystem.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionJumpForce.h"
#include "GameFramework/Character.h"
#include "TDW/Player/TDWPlayerController.h"


UTDWGameplayAbility_Leap::UTDWGameplayAbility_Leap()
{
	MaxLeapDistance = 1200.f;
	MinLeapDistance = 300.f;
	LeapHeightCurve = NewObject<UCurveFloat>();
	LeapHeightCurve->FloatCurve.UpdateOrAddKey(0.0f, 150.0f);
	LeapHeightCurve->FloatCurve.UpdateOrAddKey(1.0f, 600.0f);
	LeapDuration	= 0.7f;
}

void UTDWGameplayAbility_Leap::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	auto* Character = GetCharacterFromActorInfo();
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FVector RawTargetLocation = ResolveTargetLocation(ActorInfo, TriggerEventData);

	const float Distance = FVector::Dist(Character->GetActorLocation(), RawTargetLocation);

	if (Distance < MinLeapDistance || Distance > MaxLeapDistance)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	auto* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSys)
	{
		FNavLocation NavLoc;
		if (NavSys->ProjectPointToNavigation(RawTargetLocation, NavLoc, FVector(200.f, 200.f, 500.f)))
		{
			RawTargetLocation = NavLoc.Location;
		}
	}
	
	FVector GroundTarget;
	TargetLocation = FindGroundAtLocation(RawTargetLocation, GroundTarget) ? GroundTarget : RawTargetLocation;

	const FRotator Rotation = (TargetLocation - Character->GetActorLocation()).Rotation();
	Character->SetActorRotation(FRotator(0.f, Rotation.Yaw, 0.f));

	const float LeapHeight = ResolveJumpHeight(Distance);
	
	auto* JumpTask = UAbilityTask_ApplyRootMotionJumpForce::ApplyRootMotionJumpForce(
		this,
		NAME_None,
		Rotation,
		Distance,
		LeapHeight,
		LeapDuration,
		0.2f,
		true,
		ERootMotionFinishVelocityMode::SetVelocity,
		FVector::Zero(),
		0.0f,
		nullptr,
		nullptr
		);

	if (auto TDWPC = GetTDWPlayerControllerFromActorInfo())
	{
		TDWPC->ForceStopMovement();
	}

	JumpTask->OnLanded.AddDynamic(this, &ThisClass::OnLanded);
	JumpTask->ReadyForActivation();
}

void UTDWGameplayAbility_Leap::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UTDWGameplayAbility_Leap::OnLanded()
{
	auto* ASC = GetAbilitySystemComponentFromActorInfo();
	check(ASC);

	if (LandingAbility)
	{
		ASC->TryActivateAbilityByClass(LandingAbility);
	}
}

FVector UTDWGameplayAbility_Leap::ResolveTargetLocation(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayEventData* TriggerEventData) const
{
	AActor* Avatar = ActorInfo->AvatarActor.Get();

	if (TriggerEventData)
	{
		if (TriggerEventData->Target)
			return TriggerEventData->Target->GetActorLocation();

		if (!TriggerEventData->TargetData.Data.IsEmpty())
		{
			if (const FGameplayAbilityTargetData_LocationInfo* Loc =
				static_cast<const FGameplayAbilityTargetData_LocationInfo*>(
					TriggerEventData->TargetData.Get(0)))
			{
				return Loc->GetEndPoint();
			}
		}
	}

	if (ActorInfo->PlayerController.IsValid())
	{
		FHitResult Hit;
		ActorInfo->PlayerController->GetHitResultUnderCursor(
			ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
			return Hit.ImpactPoint;
	}

	return Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * MaxLeapDistance;
}

float UTDWGameplayAbility_Leap::ResolveJumpHeight(const float Distance) const
{
	const float Percentage = Distance / MaxLeapDistance;
	return LeapHeightCurve->GetFloatValue(Percentage);
}

bool UTDWGameplayAbility_Leap::FindGroundAtLocation(const FVector& InLocation, FVector& OutGroundLocation, float TraceUp, float TraceDown) const
{
	const UWorld* World = GetWorld();
	if (!World) return false;

	const FVector Start = InLocation + FVector::UpVector * TraceUp;
	const FVector End   = InLocation - FVector::UpVector * TraceDown;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.AddIgnoredActor(GetAvatarActorFromActorInfo());

	if (const bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		OutGroundLocation = Hit.ImpactPoint;
		return true;
	}

	return false;
}



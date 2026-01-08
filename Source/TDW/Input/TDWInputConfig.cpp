// Fill out your copyright notice in the Description page of Project Settings.


#include "TDWInputConfig.h"
#include "TDW/TDW.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TDWInputConfig)


UTDWInputConfig::UTDWInputConfig()
{
}

const UInputAction* UTDWInputConfig::FindNativeInputActionFromTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FTDWInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTDW, Error, TEXT("Could not find native input action for InputTag=%s on InputConfig=&s"), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}

const UInputAction* UTDWInputConfig::FindAbilityInputActionFromTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FTDWInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTDW, Error, TEXT("Could not find ability input action for InputTag=%s on InputConfig=&s"), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}



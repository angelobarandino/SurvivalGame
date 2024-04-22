// Fill out your copyright notice in the Description page of Project Settings.


#include "SGInputConfig.h"

#include "InputAction.h"
#include "SurvivalGame/SurvivalGame.h"

USGInputConfig::USGInputConfig(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const UInputAction* USGInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, const bool bLogIfNotFound) const
{
	for (const FSGInputAction& Entry : NativeInputAction)
	{
		if (Entry.InputAction && Entry.InputTag.MatchesTagExact(InputTag))
		{
			return Entry.InputAction.Get();
		}
	}

	if (bLogIfNotFound)
	{
		UE_LOG(LogSurvivalGame, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* USGInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogIfNotFound) const
{
	
	for (const FSGInputAction& Entry : AbilityInputAction)
	{
		if (Entry.InputAction && Entry.InputTag.MatchesTagExact(InputTag))
		{
			return Entry.InputAction.Get();
		}
	}

	if (bLogIfNotFound)
	{
		UE_LOG(LogSurvivalGame, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

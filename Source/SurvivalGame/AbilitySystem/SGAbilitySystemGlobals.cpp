// Fill out your copyright notice in the Description page of Project Settings.


#include "SGAbilitySystemGlobals.h"

#include "SGGameplayEffectContext.h"

FGameplayEffectContext* USGAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FSGGameplayEffectContext();
}

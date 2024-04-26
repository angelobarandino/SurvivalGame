#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "SGGameplayEffectContext.generated.h"

USTRUCT()
struct FSGGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FSGGameplayEffectContext::StaticStruct();
	}

	virtual FSGGameplayEffectContext* Duplicate() const override
	{
		FSGGameplayEffectContext* NewContext = new FSGGameplayEffectContext();
		*NewContext = static_cast<FSGGameplayEffectContext>(*this);
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
};

template<>
struct TStructOpsTypeTraits<FSGGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FSGGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

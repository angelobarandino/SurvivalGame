#pragma once

#include "CoreMinimal.h"
#include "InteractOption.generated.h"

class IInteractableTarget;

USTRUCT(BlueprintType)
struct FInteractOption
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IInteractableTarget> InteractableTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;
};

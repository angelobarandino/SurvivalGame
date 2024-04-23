#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "InteractOption.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UUserWidget;
class IInteractableTarget;

USTRUCT(BlueprintType)
struct FInteractOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractDuration = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IInteractableTarget> InteractableTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ActivationRequiredTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> InteractionAbilityToGrant;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> InteractionWidgetClass;
};
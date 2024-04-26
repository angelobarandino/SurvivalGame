#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/SGGameplayAbility_InteractAction.h"
#include "InteractOption.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UUserWidget;
class IInteractableTarget;

USTRUCT(BlueprintType)
struct FInteractOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText InteractText;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USGGameplayAbility_InteractAction> InteractionAbilityToGrant;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InteractInputTag;

	UPROPERTY(BlueprintReadOnly)
	float InteractDuration = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IInteractableTarget> InteractableTarget;
};

USTRUCT(BlueprintType)
struct FInteractionDefinition
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName Name;

	UPROPERTY(BlueprintReadOnly)
	TArray<FInteractOption> InteractOptions;

	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IInteractableTarget> InteractableTarget;

	UPROPERTY(BlueprintReadOnly)
	TSoftClassPtr<UUserWidget> InteractWidgetClass;
};

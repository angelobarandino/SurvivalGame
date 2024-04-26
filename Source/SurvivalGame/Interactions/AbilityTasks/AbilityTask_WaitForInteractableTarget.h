#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitForInteractableTarget.generated.h"

struct FInteractionDefinition;
struct FInteractOption;
class IInteractableTarget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionOptionsDelegate, const FInteractionDefinition&, InteractionDefinition);

UCLASS()
class UAbilityTask_WaitForInteractableTarget : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasl", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly=true))
	static UAbilityTask_WaitForInteractableTarget* WaitForInteractableTarget(UGameplayAbility* OwningAbility, float InteractionScanRange = 100.f, float InteractionScanRate = 0.1f, bool bShowDebug = false);

	virtual void Activate() override;

protected:
	UPROPERTY(BlueprintAssignable)
	FInteractionOptionsDelegate FoundInteractable;

	UPROPERTY(BlueprintAssignable)
	FInteractionOptionsDelegate LostInteractable;

	virtual void OnDestroy(bool bInOwnerFinished) override;
	
private:
	
	float InteractionScanRadius = 100.f;
	float InteractionScanRate = 0.1f;
	bool bShowDebug = false;

	TScriptInterface<IInteractableTarget> ActiveInteractableTarget;
	TMap<FObjectKey, FGameplayAbilitySpecHandle> InteractOptionsAbilityHandles;
	
	
	void OnPawnAvatarInitialized();
	void OnFoundInteractableTarget(TScriptInterface<IInteractableTarget> InteractableTarget);
	void OnLostInteractableTarget(TScriptInterface<IInteractableTarget> InteractableTarget);
	void GrantInteractionOptionsAbility(const TArray<FInteractOption>& InteractOptions);
};

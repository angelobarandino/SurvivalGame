#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitForInteractableTarget.generated.h"

class IInteractableTarget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFoundInteractableTargetDelegate);

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
	FFoundInteractableTargetDelegate FoundInteractable;

	UPROPERTY(BlueprintAssignable)
	FFoundInteractableTargetDelegate LostInteractable;

	virtual void OnDestroy(bool bInOwnerFinished) override;
	
private:
	
	float InteractionScanRadius = 100.f;
	float InteractionScanRate = 0.1f;
	bool bShowDebug = false;

	FTimerHandle TimerHandle;

	TScriptInterface<IInteractableTarget> ActiveInteractableTarget;

	void PerformTrace();
	void FoundInteractableTarget(const TScriptInterface<IInteractableTarget>& InteractableObject);
	void LostInteractableTarget();
	
};

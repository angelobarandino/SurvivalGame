// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_FindOrSpawnLootContainer.generated.h"

class AItemPickupContainer;
class UInventoryItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFoundLootContainer, const AActor*, LootContainerActor);

UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API UAbilityTask_FindOrSpawnLootContainer : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_FindOrSpawnLootContainer(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = True))
	static UAbilityTask_FindOrSpawnLootContainer* FindOrSpawnLootItemContainer(UGameplayAbility* OwningAbility, const UInventoryItemInstance* ItemInstance, const ECollisionChannel TraceChannel, const TSubclassOf<AItemPickupContainer> LootContainerClass);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FFoundLootContainer Success;
	
	UPROPERTY(BlueprintAssignable)
	FFoundLootContainer Failed;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:

	UPROPERTY(Replicated)
	TObjectPtr<AActor> LootContainer = nullptr;

	ECollisionChannel CollisionChannel;

	UPROPERTY()
	TObjectPtr<const UInventoryItemInstance> CurrentItemInstance = nullptr;

	UPROPERTY()
	TSubclassOf<AItemPickupContainer> ItemPickupContainerClass  = nullptr;

	AActor* FindItemContainerAroundPlayer(const FVector& Location) const;
	
	FVector GenerateRandomLocation(const FVector& CenterLocation) const;
	
	AActor* TrySpawnNewItemContainerAtLocation(APawn* Instigator, const FVector& SpawnLocation) const;

	void OnSuccessCallback();
	void OnFailedCallback();
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_MoveInventoryItem.h"

#include "InventorySystem/InventoryManagerComponent.h"
#include "Pickups/IPickupable.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_MoveInventoryItem)

UGameplayAbility_MoveInventoryItem::UGameplayAbility_MoveInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameplayAbility_MoveInventoryItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthority(&CurrentActivationInfo))
	{
		MoveInventoryItem(TriggerEventData->Target, TriggerEventData->OptionalObject);
	}
}

void UGameplayAbility_MoveInventoryItem::MoveInventoryItem(const AActor* TargetActor, const UObject* MoveItemObject)
{
	if (TargetActor == nullptr || MoveItemObject == nullptr)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (const UMoveInventoryItemPayload* Data = Cast<UMoveInventoryItemPayload>(MoveItemObject))
	{
		if (AItemPickupContainer* SourceActor = UPickupableStatics::FindActorByNetGUID<AItemPickupContainer>(GetWorld(), Data->SourceActorNetGUID))
		{
			UInventoryManagerComponent* SourceInventory = SourceActor->FindComponentByClass<UInventoryManagerComponent>();
			UInventoryManagerComponent* TargetInventory = TargetActor->FindComponentByClass<UInventoryManagerComponent>();
			check(SourceInventory && TargetInventory);

			TargetInventory->MoveInventoryItem(Data->OldSlot, Data->NewSlot);
			
			SourceActor->ForceNetUpdate();
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/GameplayAbility_MoveInventoryItem.h"

#include "InventorySystem/InventoryManagerComponent.h"
#include "SurvivalGame/Interactions/InteractionStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_MoveInventoryItem)

UGameplayAbility_MoveInventoryItem::UGameplayAbility_MoveInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameplayAbility_MoveInventoryItem::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthority(&CurrentActivationInfo) && TriggerEventData)
	{
		MoveInventoryItem(*TriggerEventData);
	}
}

void UGameplayAbility_MoveInventoryItem::MoveInventoryItem(const FGameplayEventData& TriggerEventData)
{
	if (TriggerEventData.OptionalObject == nullptr || !IsValid(TriggerEventData.Instigator) || !IsValid(TriggerEventData.Target))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	const AActor* PlayerActor = TriggerEventData.Instigator;
	const AActor* TargetActor = UInteractionStatics::GetCurrentInteractingActor(
		Cast<APawn>(GetAvatarActorFromActorInfo()));
	
	if (const UMoveInventoryItemPayload* Data = Cast<UMoveInventoryItemPayload>(TriggerEventData.OptionalObject))
	{
 		// If not Player, and Target is valid, then were are moving items on a none player inventory
		if (Data->MoveAction == EMoveItemActionType::Move_TargetOnly && IsValid(TargetActor))
		{
			if (UInventoryManagerComponent* TargetInventory = TargetActor->FindComponentByClass<UInventoryManagerComponent>())
			{
				TargetInventory->Server_MoveInventorItem(Data->SourceSlot, Data->TargetSlot);
			}
		}
		
		// Move an item from external inventory to player inventory
		else if (Data->MoveAction == EMoveItemActionType::Move_TargetToPlayer && IsValid(TargetActor))
		{
			UInventoryManagerComponent* PlayerInventory = PlayerActor->FindComponentByClass<UInventoryManagerComponent>();
			UInventoryManagerComponent* SourceInventory = TargetActor->FindComponentByClass<UInventoryManagerComponent>();
			check(PlayerInventory && SourceInventory);
			
			PlayerInventory->Server_AddInventoryItemFromOtherSourceWithTargetSlot(Data->TargetSlot, Data->SourceSlot, SourceInventory);
		}
		
		// Moving an item from player's inventory to external inventory
		else if (Data->MoveAction == EMoveItemActionType::Move_PlayerToTarget && IsValid(TargetActor))
		{
			UInventoryManagerComponent* PlayerInventory = PlayerActor->FindComponentByClass<UInventoryManagerComponent>();
			UInventoryManagerComponent* TargetInventory = TargetActor->FindComponentByClass<UInventoryManagerComponent>();
			check(PlayerInventory && TargetInventory);
		
			TargetInventory->Server_AddInventoryItemFromOtherSourceWithTargetSlot(Data->TargetSlot, Data->SourceSlot, PlayerInventory);
		}

		// Player is moving an item within its own inventory
		else if (Data->MoveAction == EMoveItemActionType::Move_PlayerOnly)
		{
			UInventoryManagerComponent* PlayerInventory = PlayerActor->FindComponentByClass<UInventoryManagerComponent>();
			check(PlayerInventory);
			
			PlayerInventory->Server_MoveInventorItem(Data->SourceSlot, Data->TargetSlot);
		}
	}
	
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
}

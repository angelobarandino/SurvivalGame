// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryItemSlotWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ItemDefinitionLibrary.h"
#include "ItemFragment_Inventory.h"
#include "NativeGameplayTags.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"
#include "Engine/AssetManager.h"
#include "InventorySystem/InventoryItemDragDropOperation.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventorySystem/InventoryManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Pickups/IPickupable.h"
#include "SurvivalGame/Player/SGPlayerController.h"
#include "SurvivalGame/Player/SGPlayerState.h"
#include "UI/InventoryItemDragPreview.h"
#include "UI/InventoryItemTooltip.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryItemSlotWidget)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_SurvivalGameItems_Inventory_MoveInventoryItem, "SurvivalGameItems.Inventory.MoveItem");

void UMoveInventoryItemPayload::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UInventoryItemSlotWidget::UInventoryItemSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInventoryItemSlotWidget::SetInventorySlotItemInstance(const UInventoryItemInstance* ItemInstance)
{
	if (ItemInstance == nullptr)
	{
		EmptyInventorySlot();
		return;
	}

	CurrentItemInstance = ItemInstance;
	SetInventorySlotItem(ItemInstance->GetItemDef(), ItemInstance->GetItemCount());
	SetInventoryItemTooltip();
}

void UInventoryItemSlotWidget::ClearInventorySlotItemInstance()
{
	EmptyInventorySlot();
	CurrentItemInstance = nullptr;
}

void UInventoryItemSlotWidget::SetInventoryItemTooltip()
{
	if (CurrentItemInstance && CurrentItemInstance->GetItemDef())
	{
		const UItemFragment_Inventory* InventoryFragment = UItemDefinitionLibrary::FindItemDefinitionFragment<
			UItemFragment_Inventory>(CurrentItemInstance.Get()->GetItemDef());

		if (InventoryFragment != nullptr && !InventoryFragment->TooltipWidgetClass.IsNull())
		{
			TooltipWidgetClass = OverrideTooltipWidgetClass.IsNull()
				? InventoryFragment->TooltipWidgetClass
				: OverrideTooltipWidgetClass;
			
			StreamingHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
				TooltipWidgetClass.ToSoftObjectPath(),
				FStreamableDelegate::CreateUObject(this, &ThisClass::OnTooltipWidgetLoaded),
				FStreamableManager::AsyncLoadHighPriority
			);
		}
	}
}

void UInventoryItemSlotWidget::OnTooltipWidgetLoaded()
{
	if (CurrentItemInstance.Get())
	{
		if (const TSubclassOf<UInventoryItemTooltip> UserWidgetClass = TooltipWidgetClass.Get())
		{
			if (UInventoryItemTooltip* TooltipWidget = Cast<UInventoryItemTooltip>(UWidgetBlueprintLibrary::Create(GetWorld(), UserWidgetClass, GetOwningPlayer())))
			{
				TooltipWidget->SetItemData(CurrentItemInstance->GetItemDef(), CurrentItemInstance->GetItemCount());
				SetToolTip(TooltipWidget);
			}
		}
	}

	StreamingHandle.Reset();
}

void UInventoryItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OwningActor.IsValid())
	{
		InventoryManager = OwningActor->FindComponentByClass<UInventoryManagerComponent>();

		if (InventoryManager && SlotIndex >= 0)
		{
			SetInventorySlotItemInstance(InventoryManager->FindItemInstanceInSlot(SlotIndex));
		}
	}
}

void UInventoryItemSlotWidget::NativeDestruct()
{
	InventoryManager = nullptr;
	CurrentItemInstance = nullptr;
	TooltipWidgetClass = nullptr;
	
	Super::NativeDestruct();
}

FReply UInventoryItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CurrentItemInstance)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

	return FReply::Unhandled();
}

void UInventoryItemSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CurrentItemInstance)
	{
		Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

		if (InventoryManager)
		{
			InventoryManager->SetFocusedInventoryItemSlot(CurrentItemInstance->GetItemSlot());
		}
	}
}

void UInventoryItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (CurrentItemInstance)
	{
		Super::NativeOnMouseLeave(InMouseEvent);
		
		if (InventoryManager)
		{
			InventoryManager->SetFocusedInventoryItemSlot(-1);
		}
	}
}

FReply UInventoryItemSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	
	if (CurrentItemInstance.Get() && InventoryManager && bEnableDragAndDrop)
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
		}
	}
	
	return FReply::Unhandled();
}

void UInventoryItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (CurrentItemInstance.Get() && InventoryManager && bEnableDragAndDrop)
	{
		if (UUserWidget* PreviewWidget = CreateDragPreview())
		{
			if (UInventoryItemDragDropOperation* DragDropOperation = Cast<UInventoryItemDragDropOperation>(
				UWidgetBlueprintLibrary::CreateDragDropOperation(UInventoryItemDragDropOperation::StaticClass())))
			{
				DragDropOperation->DefaultDragVisual = PreviewWidget;
				DragDropOperation->DraggedItemInstance = CurrentItemInstance.Get();
				DragDropOperation->DraggedItemSlot = CurrentItemInstance->GetItemSlot();
				DragDropOperation->SourceInventory = InventoryManager;
				DragDropOperation->SourceActor = OwningActor;
				OutOperation = DragDropOperation;
			}
		}
	}
}

bool UInventoryItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (const UInventoryItemDragDropOperation* Operation = Cast<UInventoryItemDragDropOperation>(InOperation))
	{
		UInventoryManagerComponent* SourceInventoryManager = Operation->SourceInventory;
		
		if (ASGPlayerState* PlayerState = Cast<ASGPlayerState>(GetOwningPlayerState()))
		{
			int32 ActorNetGUID = -1;
			if (const IPickupable* Pickupable = Cast<IPickupable>(Operation->SourceActor.Get()))
			{
				ActorNetGUID = Pickupable->GetActorNetGUID();
			}
			
			UMoveInventoryItemPayload* MoveItemData = NewObject<UMoveInventoryItemPayload>();
			MoveItemData->SourceActorNetGUID = ActorNetGUID;
			MoveItemData->ItemInstance = Operation->DraggedItemInstance;
			MoveItemData->OldSlot = Operation->DraggedItemSlot;
			MoveItemData->NewSlot = SlotIndex;
			
			FGameplayEventData Payload;
			Payload.EventTag = TAG_SurvivalGameItems_Inventory_MoveInventoryItem;
			Payload.OptionalObject = MoveItemData;
			Payload.Instigator = GetOwningPlayer();
			Payload.Target = OwningActor.Get();

			PlayerState->Server_ActivateActorAbilityByEvent(TAG_SurvivalGameItems_Inventory_MoveInventoryItem, Payload);
		}

		if (InventoryManager == SourceInventoryManager)
		{
			// if (IPickupable* PickupableActor = Cast<IPickupable>(OwningActor))
			// {
			// 	PickupableActor->MoveInventorItem(Operation->DraggedItemSlot, SlotIndex);
			// 	return true;
			// }
			//
			// SourceInventoryManager->MoveInventorItem(Operation->DraggedItemSlot, SlotIndex);
			// SourceInventoryManager->SetFocusedInventoryItemSlot(SlotIndex);
		}

		if (GetOwningPlayer() != OwningActor.Get())
		{
			OwningActor->ForceNetUpdate();
		}

		return true;
	}
	
	return false;
}

UUserWidget* UInventoryItemSlotWidget::CreateDragPreview()
{
	if (DragPreviewWidgetClass == nullptr)
	{
		return nullptr;
	}

	if (UInventoryItemDragPreview* ItemDragPreview = Cast<UInventoryItemDragPreview>(UWidgetBlueprintLibrary::Create(this, DragPreviewWidgetClass, GetOwningPlayer())))
	{
		ItemDragPreview->ItemInstance = CurrentItemInstance;
		return ItemDragPreview;
	}

	return nullptr;
}

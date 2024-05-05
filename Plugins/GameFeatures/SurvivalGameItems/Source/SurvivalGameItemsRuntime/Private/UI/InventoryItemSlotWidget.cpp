// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryItemSlotWidget.h"

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
#include "Pickups/IPickupable.h"
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

void UInventoryItemSlotWidget::SetInventorySlotItemInstance(const UInventoryItemInstance* InItemInstance)
{
	if (InItemInstance == nullptr)
	{
		EmptyInventorySlot();
		return;
	}

	ItemInstance = InItemInstance;
	SetInventorySlotItem(InItemInstance->GetItemDef(), InItemInstance->GetItemCount());
	SetInventoryItemTooltip();
}

void UInventoryItemSlotWidget::ClearInventorySlotItemInstance()
{
	EmptyInventorySlot();
	ItemInstance = nullptr;
}

void UInventoryItemSlotWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UInventoryItemSlotWidget::BeginDestroy()
{
	Super::BeginDestroy();
}

void UInventoryItemSlotWidget::SetInventoryItemTooltip()
{
	if (ItemInstance && ItemInstance->GetItemDef())
	{
		const UItemFragment_Inventory* InventoryFragment = UItemDefinitionLibrary::FindItemDefinitionFragment<
			UItemFragment_Inventory>(ItemInstance->GetItemDef());

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
	if (ItemInstance)
	{
		if (const TSubclassOf<UInventoryItemTooltip> UserWidgetClass = TooltipWidgetClass.Get())
		{
			if (UInventoryItemTooltip* TooltipWidget = Cast<UInventoryItemTooltip>(UWidgetBlueprintLibrary::Create(GetWorld(), UserWidgetClass, GetOwningPlayer())))
			{
				TooltipWidget->SetItemData(ItemInstance->GetItemDef(), ItemInstance->GetItemCount());
				SetToolTip(TooltipWidget);
			}
		}
	}

	StreamingHandle.Reset();
}

void UInventoryItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemInstance)
	{
		SetInventorySlotItem(ItemInstance->GetItemDef(), ItemInstance->GetItemCount());
		SetInventoryItemTooltip();
	}
	else
	{
		ClearInventorySlotItemInstance();
	}
}

void UInventoryItemSlotWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	TooltipWidgetClass = nullptr;
	ItemInstance = nullptr;
	
	OwningInventoryManager.Reset();
	OwningActor.Reset();
}

FReply UInventoryItemSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ItemInstance)
	{
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

	return FReply::Unhandled();
}

void UInventoryItemSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ItemInstance && bEnableSetFocusItem)
	{
		Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

		if (OwningInventoryManager.IsValid())
		{
			OwningInventoryManager->SetFocusedInventoryItemSlot(ItemInstance->GetItemSlot());
		}
	}
}

void UInventoryItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (ItemInstance && bEnableSetFocusItem)
	{
		Super::NativeOnMouseLeave(InMouseEvent);
		
		if (OwningInventoryManager.IsValid())
		{
			OwningInventoryManager->SetFocusedInventoryItemSlot(-1);
		}
	}
}

FReply UInventoryItemSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	
	if (IsValid(ItemInstance) && OwningInventoryManager.IsValid() && bEnableDragAndDrop)
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

	if (IsValid(ItemInstance) && OwningInventoryManager.IsValid() && bEnableDragAndDrop)
	{
		if (UUserWidget* PreviewWidget = CreateDragPreview())
		{
			if (UInventoryItemDragDropOperation* DragDropOperation = Cast<UInventoryItemDragDropOperation>(
				UWidgetBlueprintLibrary::CreateDragDropOperation(UInventoryItemDragDropOperation::StaticClass())))
			{
				DragDropOperation->DefaultDragVisual = PreviewWidget;
				DragDropOperation->DraggedItemInstance = ItemInstance;
				DragDropOperation->DraggedItemSlot = ItemInstance->GetItemSlot();
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
		if (ASGPlayerState* PlayerState = Cast<ASGPlayerState>(GetOwningPlayerState()))
		{
			FGuid SourceActorNetGUID = FGuid();
			if (const IPickupable* Pickupable = Cast<IPickupable>(Operation->SourceActor.Get()))
			{
				SourceActorNetGUID = Pickupable->GetActorNetGUID();
			}

			FGuid TargetActorNetGUID = FGuid();
			if (const IPickupable* Pickupable = Cast<IPickupable>(OwningActor.Get()))
			{
				TargetActorNetGUID = Pickupable->GetActorNetGUID();
			}
			
			UMoveInventoryItemPayload* MoveItemData = NewObject<UMoveInventoryItemPayload>();
			MoveItemData->bPlayerInventory = GetOwningPlayer() == OwningActor || GetOwningPlayer() == Operation->SourceActor;
			MoveItemData->SourceActorNetGUID = SourceActorNetGUID;
			MoveItemData->TargetActorNetGUID = TargetActorNetGUID;
			MoveItemData->SourceSlot = Operation->DraggedItemSlot;
			MoveItemData->TargetSlot = SlotIndex;
			
			FGameplayEventData Payload;
			Payload.EventTag = TAG_SurvivalGameItems_Inventory_MoveInventoryItem;
			Payload.OptionalObject = MoveItemData;
			Payload.Instigator = GetOwningPlayer();
			Payload.Target = OwningActor.Get();

			PlayerState->Server_ActivateActorAbilityByEvent(TAG_SurvivalGameItems_Inventory_MoveInventoryItem, Payload);
			return true;
		}
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
		ItemDragPreview->ItemInstance = ItemInstance;
		return ItemDragPreview;
	}

	return nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryItemSlotWidget.h"

#include "ItemDefinitionLibrary.h"
#include "ItemFragment_Inventory.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"
#include "Engine/AssetManager.h"
#include "InventorySystem/InventoryItemDragDropOperation.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventorySystem/InventoryManagerComponent.h"
#include "UI/InventoryItemDragPreview.h"
#include "UI/InventoryItemTooltip.h"

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
			TooltipWidgetSoftClass = InventoryFragment->TooltipWidgetClass;
			StreamingHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
				TooltipWidgetSoftClass.ToSoftObjectPath(),
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
		if (const TSubclassOf<UInventoryItemTooltip> UserWidgetClass = TooltipWidgetSoftClass.Get())
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

	if (const APlayerController* PC = GetOwningPlayer())
	{
		InventoryManager = PC->FindComponentByClass<UInventoryManagerComponent>();
	}
}

void UInventoryItemSlotWidget::NativeDestruct()
{
	InventoryManager = nullptr;
	CurrentItemInstance = nullptr;
	TooltipWidgetSoftClass = nullptr;
	
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

	if (CurrentItemInstance.Get() && InventoryManager)
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

	if (CurrentItemInstance.Get() && InventoryManager)
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

		if (InventoryManager == SourceInventoryManager)
		{
			InventoryManager->MoveInventorItem(Operation->DraggedItemSlot, SlotIndex);
		}

		if (InventoryManager)
		{
			InventoryManager->SetFocusedInventoryItemSlot(SlotIndex);
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

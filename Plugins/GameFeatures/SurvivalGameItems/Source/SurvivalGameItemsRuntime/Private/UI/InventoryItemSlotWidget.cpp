// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryItemSlotWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "InventorySystem/InventoryItemDragDropOperation.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventorySystem/InventoryManagerComponent.h"
#include "UI/InventoryItemDragPreview.h"

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
	SetInventorySlotItem(ItemInstance);
}

void UInventoryItemSlotWidget::SetInventorySlotItem_Implementation(const UInventoryItemInstance* ItemInstance)
{
}

void UInventoryItemSlotWidget::EmptyInventorySlot_Implementation()
{
}

void UInventoryItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const APlayerController* PC = GetOwningPlayer())
	{
		InventoryManager = PC->FindComponentByClass<UInventoryManagerComponent>();
	}
}

FReply UInventoryItemSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	if (CurrentItemInstance && InventoryManager)
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

	if (CurrentItemInstance == nullptr || InventoryManager == nullptr)
	{
		return;
	}
	
	if (UUserWidget* PreviewWidget = CreateDragPreview())
	{
		UInventoryItemDragDropOperation* DragDropOperation = Cast<UInventoryItemDragDropOperation>(
			UWidgetBlueprintLibrary::CreateDragDropOperation(UInventoryItemDragDropOperation::StaticClass()));
		
		if (DragDropOperation)
		{
			DragDropOperation->DefaultDragVisual = PreviewWidget;
			DragDropOperation->DraggedItemInstance = CurrentItemInstance;
			DragDropOperation->DraggedItemSlot = CurrentItemInstance->GetItemSlot();
			DragDropOperation->SourceInventory = InventoryManager;
			OutOperation = DragDropOperation;
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
			return InventoryManager->MoveInventorItem(Operation->DraggedItemSlot, SlotIndex);
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
		ItemDragPreview->ItemInstance = CurrentItemInstance;
		return ItemDragPreview;
	}

	return nullptr;
}

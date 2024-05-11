// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryItemSlotWidget.h"

#include "AbilitySystemComponent.h"
#include "ItemDefinitionLibrary.h"
#include "ItemFragment_Inventory.h"
#include "NativeGameplayTags.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"
#include "Components/PawnItemManagerComponent.h"
#include "Engine/AssetManager.h"
#include "InventorySystem/InventoryItemDragDropOperation.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventorySystem/InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SurvivalGame/Player/SGPlayerState.h"
#include "UI/InventoryGrid.h"
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

	InventoryGrid = nullptr;
	TooltipWidgetClass = nullptr;
	ItemInstance = nullptr;
	
	OwningInventoryManager.Reset();
	OwningActor = nullptr;
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
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	if (ItemInstance)
	{
		UPawnItemManagerStatics::SetFocusedInventoryItem(GetOwningPlayerPawn(), ItemInstance->GetItemSlot(), IsPlayerOwner());

		if (OwningInventoryManager.IsValid() && bEnableSetFocusItem)
		{
			OwningInventoryManager->SetFocusedInventoryItemSlot(ItemInstance->GetItemSlot());
		}
	}
}

void UInventoryItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	if (ItemInstance)
	{
		UPawnItemManagerStatics::SetFocusedInventoryItem(GetOwningPlayerPawn(), -1,  IsPlayerOwner());
		
		if (OwningInventoryManager.IsValid() && bEnableSetFocusItem)
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
		
		UPawnItemManagerStatics::SetFocusedInventoryItem(GetOwningPlayerPawn(), SlotIndex,  IsPlayerOwner());
		
		if (ASGPlayerState* PlayerState = Cast<ASGPlayerState>(GetOwningPlayerState()))
		{
			UMoveInventoryItemPayload* MoveItemData = NewObject<UMoveInventoryItemPayload>();
			MoveItemData->SourceSlot = Operation->DraggedItemSlot;
			MoveItemData->TargetSlot = SlotIndex;

			// If dropping on same owner, and it is player's inventory
			if (OwningActor == Operation->SourceActor && GetOwningPlayer() == Operation->SourceActor)
			{
				MoveItemData->MoveAction = EMoveItemActionType::Move_PlayerOnly;
			}
			
			// If dropping on same owner, and it's not the players inventory
			if (OwningActor == Operation->SourceActor && GetOwningPlayer() != Operation->SourceActor)
			{
				MoveItemData->MoveAction = EMoveItemActionType::Move_TargetOnly;
			}

			// If dropping on player inventory where the item is from other source
			else if (OwningActor == GetOwningPlayer() && GetOwningPlayer() != Operation->SourceActor)
			{
				MoveItemData->MoveAction = EMoveItemActionType::Move_TargetToPlayer;
			}

			// If dropping an item on a non player inventory where the item is from the player
			else if (OwningActor != GetOwningPlayer() && GetOwningPlayer() == Operation->SourceActor)
			{
				MoveItemData->MoveAction = EMoveItemActionType::Move_PlayerToTarget;
			}
			
			FGameplayEventData Payload;
			Payload.EventTag = TAG_SurvivalGameItems_Inventory_MoveInventoryItem;
			Payload.OptionalObject = MoveItemData;
			Payload.Instigator = GetOwningPlayer();
			Payload.Target = OwningActor;

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
		if (InventoryGrid)
		{
			ItemDragPreview->SizeBoxRoot->SetWidthOverride(InventoryGrid->SlotSize);
			ItemDragPreview->SizeBoxRoot->SetHeightOverride(InventoryGrid->SlotSize);
		}

		ItemDragPreview->ItemInstance = ItemInstance;
		return ItemDragPreview;
	}

	return nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryGrid.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "InventorySystem/InventoryManagerComponent.h"
#include "UI/InventoryItemSlotWidget.h"

UInventoryGrid::UInventoryGrid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UInventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();

	WidgetPool.SetWorld(GetWorld());
}

void UInventoryGrid::NativeDestruct()
{
	Super::NativeDestruct();
	
	WidgetPool.Release(InventorySlots);
	InventorySlots.Empty();
	
	OwningInventoryManager = nullptr;
	OwningActor.Reset();
}

void UInventoryGrid::GenerateInventoryGridFromOwningActor(AActor* InOwningActor)
{
	if (InOwningActor)
	{
		OwningActor = InOwningActor;
		OnSetOwningActor();

		OwningInventoryManager = InOwningActor->FindComponentByClass<UInventoryManagerComponent>();
		if (OwningInventoryManager)
		{
			BuildInventorySlots(OwningInventoryManager->GetMaxInventorySize());
		}
	}
}

void UInventoryGrid::ReleaseSlateResources(bool bReleaseChildren)
{
	WidgetPool.ReleaseAllSlateResources();
	
	Super::ReleaseSlateResources(bReleaseChildren);
}

void UInventoryGrid::BuildInventorySlots(const int32 MaxInventorySize)
{
	if (InventoryGrid->HasAnyChildren())
	{
		InventoryGrid->ClearChildren();
	}
	
	int32 Row = 0, Column = 0;
	for (int SlotIndex = 0; SlotIndex < MaxInventorySize; ++SlotIndex)
	{
		if (SlotIndex > 0 && SlotIndex % NumOfGridColumn == 0)
		{
			Column = 0;
			Row++;
		}
		
		CreateInventorySlot(InventorySlotClass, SlotIndex, Row, Column);
		Column++;
	}
}

void UInventoryGrid::CreateInventorySlot(const TSubclassOf<UInventoryItemSlotWidget> SlotClass, const int32 SlotIndex, const int32 Row, const int32 Column)
{
	check(InventoryGrid);

	if (SlotClass)
	{
		if (IsDesignTime())
		{
			if (UInventoryItemSlotWidget* SlotWidget = Cast<UInventoryItemSlotWidget>(UWidgetBlueprintLibrary::Create(this, SlotClass, GetOwningPlayer())))
			{
				if (UUniformGridSlot* GridSlot = InventoryGrid->AddChildToUniformGrid(SlotWidget, Row, Column))
				{
					GridSlot->SetHorizontalAlignment(HAlign_Fill);
					GridSlot->SetVerticalAlignment(VAlign_Fill);
				}
			}

			return;
		}
		
		if (OwningInventoryManager)
		{
			if (UInventoryItemSlotWidget* SlotWidget = Cast<UInventoryItemSlotWidget>(UWidgetBlueprintLibrary::Create(this, SlotClass, GetOwningPlayer())))
			{
				SlotWidget->SlotIndex = SlotIndex;
				SlotWidget->bEnableDragAndDrop = bEnableDragAndDrop;
				SlotWidget->bEnableSetFocusItem = bEnableSetFocusItem;
				SlotWidget->OverrideTooltipWidgetClass = TooltipWidgetClass;

				SlotWidget->OwningActor = OwningActor;
				SlotWidget->OwningInventoryManager = OwningInventoryManager;
				SlotWidget->ItemInstance = OwningInventoryManager->FindItemInstanceInSlot(SlotIndex);
				InventorySlots.Add(SlotWidget);
				
				if (UUniformGridSlot* GridSlot = InventoryGrid->AddChildToUniformGrid(SlotWidget, Row, Column))
				{
					GridSlot->SetHorizontalAlignment(HAlign_Fill);
					GridSlot->SetVerticalAlignment(VAlign_Fill);
				}
			}
		}
	}
}

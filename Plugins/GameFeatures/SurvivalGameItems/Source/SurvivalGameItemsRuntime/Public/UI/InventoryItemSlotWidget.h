// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventoryItemSlotWidget.generated.h"


struct FStreamableHandle;
class UInventoryItemTooltip;
class UInventoryManagerComponent;
class UInventoryItemDragPreview;

UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API UInventoryItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryItemSlotWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetInventorySlotItemInstance(const UInventoryItemInstance* ItemInstance);
	
	UFUNCTION(BlueprintCallable)
	void ClearInventorySlotItemInstance();
	
	UFUNCTION(BlueprintCallable)
	void SetInventorySlotIndex(const int32 InSlotIndex)
	{
		SlotIndex = InSlotIndex;
	}
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetInventorySlotItem(TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount);
	
	UFUNCTION(BlueprintImplementableEvent)
	void EmptyInventorySlot();
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Item")
	TSubclassOf<UInventoryItemDragPreview> DragPreviewWidgetClass;

	void SetInventoryItemTooltip();
	void OnTooltipWidgetLoaded();

	// ~Begin UUserWidget
	virtual void NativeConstruct() override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// ~End UUserWidget

private:
	TSoftClassPtr<UInventoryItemTooltip> TooltipWidgetSoftClass;
	TSharedPtr<FStreamableHandle> StreamingHandle;

	UPROPERTY()
	int32 SlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UInventoryManagerComponent> InventoryManager;

	UPROPERTY()
	TWeakObjectPtr<const UInventoryItemInstance> CurrentItemInstance;

	UUserWidget* CreateDragPreview();
};

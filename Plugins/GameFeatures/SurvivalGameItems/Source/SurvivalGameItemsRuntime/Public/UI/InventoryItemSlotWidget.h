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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemHoverChange, const UInventoryItemInstance*, ItemInstance);

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
	
	UPROPERTY(BlueprintAssignable)
	FInventoryItemHoverChange OnItemHoverActive;

	UPROPERTY(BlueprintAssignable)
	FInventoryItemHoverChange OnItemHoverEnded;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetInventorySlotItem(TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount);
	
	UFUNCTION(BlueprintImplementableEvent)
	void EmptyInventorySlot();
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Item")
	TSubclassOf<UInventoryItemDragPreview> DragPreviewWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryManagerComponent> InventoryManager;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const UInventoryItemInstance> CurrentItemInstance;

	
	void SetInventoryItemTooltip();
	void OnTooltipWidgetLoaded();

	// ~Begin UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// ~End UUserWidget

private:
	TSoftClassPtr<UInventoryItemTooltip> TooltipWidgetSoftClass;
	TSharedPtr<FStreamableHandle> StreamingHandle;

	UPROPERTY()
	int32 SlotIndex = -1;
	UFUNCTION()
	UUserWidget* CreateDragPreview();
};

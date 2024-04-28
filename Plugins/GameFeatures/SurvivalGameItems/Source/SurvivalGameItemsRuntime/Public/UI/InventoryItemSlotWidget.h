// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlotWidget.generated.h"


class UInventoryManagerComponent;
class UInventoryItemDragPreview;
class UInventoryItemInstance;

UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API UInventoryItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryItemSlotWidget(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	void SetInventorySlotItemInstance(const UInventoryItemInstance* ItemInstance);
	
	UFUNCTION(BlueprintCallable)
	void SetInventorySlotIndex(const int32 InSlotIndex)
	{
		SlotIndex = InSlotIndex;
	}
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EmptyInventorySlot();

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetInventorySlotItem(const UInventoryItemInstance* ItemInstance);
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Item")
	TSubclassOf<UInventoryItemDragPreview> DragPreviewWidgetClass;


	// ~Begin UUserWidget
	virtual void NativeConstruct() override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	// ~End UUserWidget
	
private:
	UPROPERTY()
	int32 SlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UInventoryManagerComponent> InventoryManager;

	UPROPERTY()
	TObjectPtr<const UInventoryItemInstance> CurrentItemInstance;

	UUserWidget* CreateDragPreview();
};

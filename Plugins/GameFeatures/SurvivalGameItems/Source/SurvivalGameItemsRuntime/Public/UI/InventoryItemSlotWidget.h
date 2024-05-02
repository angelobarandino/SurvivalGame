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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnableDragAndDrop = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UInventoryItemTooltip> OverrideTooltipWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	TWeakObjectPtr<AActor> OwningActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	int32 SlotIndex = -1;
	
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
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
	TSoftClassPtr<UInventoryItemTooltip> TooltipWidgetClass;
	TSharedPtr<FStreamableHandle> StreamingHandle;


	UFUNCTION()
	UUserWidget* CreateDragPreview();
};

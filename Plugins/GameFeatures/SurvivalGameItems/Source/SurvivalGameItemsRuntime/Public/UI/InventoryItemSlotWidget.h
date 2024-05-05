// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/InventoryItemInstance.h"
#include "InventoryItemSlotWidget.generated.h"


class UInventoryGrid;
struct FStreamableHandle;
class UInventoryItemTooltip;
class UInventoryManagerComponent;
class UInventoryItemDragPreview;

USTRUCT(Blueprintable)
struct FMoveItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryManagerComponent> TargetInventoryManager;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryManagerComponent> SourceInventoryManager;

};

UCLASS(Blueprintable)
class UMoveInventoryItemPayload : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	FGuid SourceActorNetGUID;
	FGuid TargetActorNetGUID;
	int32 SourceSlot;
	int32 TargetSlot;
	bool bPlayerInventory;
};

UCLASS()
class SURVIVALGAMEITEMSRUNTIME_API UInventoryItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryItemSlotWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetInventorySlotItemInstance(const UInventoryItemInstance* InItemInstance);
	
	UFUNCTION(BlueprintCallable)
	void ClearInventorySlotItemInstance();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	TWeakObjectPtr<AActor> OwningActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	int32 SlotIndex = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnableDragAndDrop = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnableSetFocusItem = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UInventoryItemTooltip> OverrideTooltipWidgetClass;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
	virtual void BeginDestroy() override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInventorySlotItem(TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount);
	
	UFUNCTION(BlueprintImplementableEvent)
	void EmptyInventorySlot();
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Item")
	TSubclassOf<UInventoryItemDragPreview> DragPreviewWidgetClass;
	
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
	friend UInventoryGrid;

	UPROPERTY()
	TObjectPtr<const UInventoryItemInstance> ItemInstance;
	
	TWeakObjectPtr<UInventoryManagerComponent> OwningInventoryManager;
	
	TSoftClassPtr<UInventoryItemTooltip> TooltipWidgetClass;
	TSharedPtr<FStreamableHandle> StreamingHandle;

	UFUNCTION()
	UUserWidget* CreateDragPreview();
};

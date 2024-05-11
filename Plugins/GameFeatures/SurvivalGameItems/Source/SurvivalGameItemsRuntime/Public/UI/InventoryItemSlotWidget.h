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

UENUM()
enum class EMoveItemActionType
{
	Move_PlayerOnly,
	Move_TargetOnly,
	Move_PlayerToTarget,
	Move_TargetToPlayer
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
	
	EMoveItemActionType MoveAction;
	int32 SourceSlot;
	int32 TargetSlot;
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
	TObjectPtr<const AActor> OwningActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn))
	int32 SlotIndex = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnableDragAndDrop = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnableSetFocusItem = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UInventoryItemTooltip> OverrideTooltipWidgetClass;

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInventorySlotItem(TSubclassOf<UItemDefinition> ItemDef, const int32 ItemCount);
	
	UFUNCTION(BlueprintImplementableEvent)
	void EmptyInventorySlot();

	UFUNCTION(BlueprintCallable)
	bool IsPlayerOwner() const
	{
		return OwningActor == GetOwningPlayer();
	}
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Item")
	TSubclassOf<UInventoryItemDragPreview> DragPreviewWidgetClass;
	
private:
	friend UInventoryGrid;
	
	UPROPERTY()
	TObjectPtr<const UInventoryGrid> InventoryGrid;

	UPROPERTY()
	TObjectPtr<const UInventoryItemInstance> ItemInstance;

	TWeakObjectPtr<UInventoryManagerComponent> OwningInventoryManager;
	
	TSoftClassPtr<UInventoryItemTooltip> TooltipWidgetClass;
	TSharedPtr<FStreamableHandle> StreamingHandle;

	UFUNCTION()
	UUserWidget* CreateDragPreview();
	
};

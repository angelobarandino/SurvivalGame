// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/UserWidgetPool.h"
#include "InventorySystem/InventoryManagerComponent.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "InventoryGrid.generated.h"

class UInventoryItemTooltip;
class UUniformGridPanel;
class UInventoryItemSlotWidget;
class UInventoryItemInstance;


UCLASS(BlueprintType)
class SURVIVALGAMEITEMSRUNTIME_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryGrid(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryGrid")
	float SlotPadding = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryGrid")
	float SlotSize = 90;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryGrid")
	int32 NumOfGridColumn = 7;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryGrid")
	bool bEnableDragAndDrop = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryGrid")
	bool bEnableSetFocusItem = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryGrid")
	TSoftClassPtr<UInventoryItemTooltip> TooltipWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InventoryGrid")
	TSubclassOf<UInventoryItemSlotWidget> InventorySlotClass;

	UFUNCTION(BlueprintCallable, Category = "InventoryGrid")
	void GenerateInventoryGridFromOwningActor(AActor* InOwningActor);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativePreConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "InventoryGrid")
	void OnSetOwningActor();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "InventoryGrid")
	UInventoryManagerComponent* GetOwningInventoryManager() const
	{
		return OwningInventoryManager.Get();
	}

	UFUNCTION(BlueprintCallable, Category = "InventoryGrid")
	void BuildInventorySlots(const int32 MaxInventorySize);
	
	UFUNCTION(BlueprintCallable, Category = "InventoryGrid")
	void CreateInventorySlot(const TSubclassOf<UInventoryItemSlotWidget> SlotClass, const int32 SlotIndex, const int32 Row, const int32 Column);

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category = "InventoryGrid")
	TObjectPtr<UUniformGridPanel> InventoryGrid;
	
	UPROPERTY(BlueprintReadOnly, Category = "InventoryGrid")
	TWeakObjectPtr<AActor> OwningActor;
	
private:
	FUserWidgetPool WidgetPool;

	UPROPERTY()
	TObjectPtr<UInventoryManagerComponent> OwningInventoryManager;

	UPROPERTY()
	TArray<UUserWidget*> InventorySlots;
};

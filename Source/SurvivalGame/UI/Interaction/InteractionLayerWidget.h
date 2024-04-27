
#pragma once

#include "CoreMinimal.h"
#include "AsyncMixin.h"
#include "Blueprint/UserWidgetPool.h"
#include "Components/CanvasPanel.h"
#include "InteractionLayerWidget.generated.h"


class UInteractionDescriptor;
class UCanvasPanel;

UCLASS()
class UInteractionLayer : public UCanvasPanel, public FAsyncMixin
{
	GENERATED_BODY()

public:
	UInteractionLayer(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void OnInitialize();

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	FUserWidgetPool InteractionWidgetPool;
	FTimerHandle ProjectTimerHandle;
	FVector ScreenPositionWithDepth;

	TWeakObjectPtr<UCanvasPanelSlot> CurrentInteractionSlot;
	TObjectPtr<UInteractionDescriptor> CurrentInteractionDescriptor;
	
	void ShowInteraction(UInteractionDescriptor* InteractionDescriptor);
	void HideInteraction(UInteractionDescriptor* InteractionDescriptor);

	void StartProject();
	bool Project(const UInteractionDescriptor* IndicatorDescriptor, const FSceneViewProjectionData& InProjectionData, const FVector2f& ScreenSize, FVector& OutScreenPositionWithDepth) const;

};

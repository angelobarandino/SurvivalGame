
#include "InteractionLayerWidget.h"

#include "AsyncMixin.h"
#include "InteractionDescriptor.h"
#include "InteractionManagerComponent.h"
#include "InteractionWidgetInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractionLayerWidget)

UInteractionLayer::UInteractionLayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsVariable = true;
	UWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInteractionLayer::OnInitialize()
{
	if (const APlayerController* PC = GetOwningPlayer())
	{
		InteractionWidgetPool.SetWorld(PC->GetWorld());
		
		if (UInteractionManagerComponent* InteractionManager = PC->FindComponentByClass<UInteractionManagerComponent>())
		{
			InteractionManager->OnShowInteraction.BindUObject(this, &ThisClass::ShowInteraction);
			InteractionManager->OnHideInteraction.BindUObject(this, &ThisClass::HideInteraction);

		}
	}
}

void UInteractionLayer::ReleaseSlateResources(bool bReleaseChildren)
{
	if (CurrentInteractionSlot.Get())
	{
		CurrentInteractionSlot.Get()->ReleaseSlateResources(bReleaseChildren);
	}
	
	Super::ReleaseSlateResources(bReleaseChildren);
}

TSharedRef<SWidget> UInteractionLayer::RebuildWidget()
{
	if (!IsDesignTime())
	{
		return Super::RebuildWidget();
	}
	
	return SNew(SBox);
}

void UInteractionLayer::ShowInteraction(UInteractionDescriptor* InteractionDescriptor)
{
	ClearChildren();

	if (InteractionDescriptor)
	{
		const TSoftClassPtr<UUserWidget> InteractionClass = InteractionDescriptor->GetInteractionWidgetClass();
		if (!InteractionClass.IsNull())
		{
			CurrentInteractionDescriptor = InteractionDescriptor;
			
			AsyncLoad(InteractionClass, [this, InteractionClass, InteractionDescriptor]()
			{
				if (UInteractionDescriptor* Interaction = InteractionDescriptor)
				{
					if (UUserWidget* InteractionWidget = InteractionWidgetPool.GetOrCreateInstance(TSubclassOf<UUserWidget>(InteractionClass.LoadSynchronous())))
					{
						if (InteractionWidget->Implements<UInteractionWidgetInterface>())
						{
							IInteractionWidgetInterface::Execute_BindInteraction(InteractionWidget, Interaction);
						}
						
						Interaction->InteractionWidget = InteractionWidget;
						CurrentInteractionSlot = AddChildToCanvas(InteractionWidget);
						CurrentInteractionSlot->SetAlignment(FVector2D(0.f, 0.5f));
						CurrentInteractionSlot->SetAutoSize(true);
						
						StartProject();

						GetWorld()->GetTimerManager().ClearTimer(ProjectTimerHandle);
						GetWorld()->GetTimerManager().SetTimer(ProjectTimerHandle, this, &ThisClass::StartProject, 0.01f, true);
					}
				}
			});

			StartAsyncLoading();
		}
	}
}

void UInteractionLayer::HideInteraction(UInteractionDescriptor* InteractionDescriptor)
{
	GetWorld()->GetTimerManager().ClearTimer(ProjectTimerHandle);

	if (InteractionDescriptor)
	{
		if (UUserWidget* InteractionWidget = InteractionDescriptor->InteractionWidget.Get())
		{
			if (InteractionWidget->Implements<UInteractionWidgetInterface>())
			{
				IInteractionWidgetInterface::Execute_UnbindInteraction(InteractionWidget, InteractionDescriptor);
			}

			RemoveChild(InteractionDescriptor->InteractionWidget.Get());
			
			InteractionDescriptor->InteractionWidget = nullptr;
			InteractionWidgetPool.Release(InteractionWidget);
		}
	}
	
	CurrentInteractionDescriptor = nullptr;
}

void UInteractionLayer::StartProject()
{
	if (CurrentInteractionDescriptor != nullptr && CurrentInteractionSlot != nullptr)
	{
		if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
		{
			FSceneViewProjectionData ProjectionData;
			if (LocalPlayer->GetProjectionData(LocalPlayer->ViewportClient->Viewport, ProjectionData))
			{
				Project(CurrentInteractionDescriptor, ProjectionData, GetCachedGeometry().GetLocalSize(), ScreenPositionWithDepth);
				CurrentInteractionSlot.Get()->SetPosition(FVector2D(ScreenPositionWithDepth));
			}
		}
	}
}

bool UInteractionLayer::Project(const UInteractionDescriptor* IndicatorDescriptor, const FSceneViewProjectionData& InProjectionData, const FVector2f& ScreenSize, FVector& OutScreenPositionWithDepth) const
{
	if (!IsValid(IndicatorDescriptor))
	{
		return false;
	}

	const FVector ProjectWorldLocation = IndicatorDescriptor->GetWorldLocation();
	
	FVector2D OutScreenSpacePosition;
	const bool bInFrontOfCamera = ULocalPlayer::GetPixelPoint(InProjectionData, ProjectWorldLocation, OutScreenSpacePosition, &ScreenSize);

	OutScreenSpacePosition.X += IndicatorDescriptor->GetScreenSpaceOffset().X * (bInFrontOfCamera ? 1 : -1);
	OutScreenSpacePosition.Y += IndicatorDescriptor->GetScreenSpaceOffset().Y;

	if (!bInFrontOfCamera && FBox2f(FVector2f::Zero(), ScreenSize).IsInside(FVector2f(OutScreenSpacePosition)))
	{
		const FVector2f CenterToPosition = (FVector2f(OutScreenSpacePosition) - (ScreenSize / 2)).GetSafeNormal();
		OutScreenSpacePosition = FVector2D((ScreenSize / 2) + CenterToPosition * ScreenSize);
	}

	OutScreenPositionWithDepth = FVector(OutScreenSpacePosition.X, OutScreenSpacePosition.Y, FVector::Dist(InProjectionData.ViewOrigin, ProjectWorldLocation));
	return true;
}

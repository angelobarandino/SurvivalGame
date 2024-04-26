// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "GameFeatureAction_AddWidgets.generated.h"

struct FComponentRequestHandle;
class UCommonActivatableWidget;

USTRUCT()
struct FHUDLayoutRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "UI", meta = (AssetBundles="Client"))
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (ExposeFunctionCategories="UI.Layer"))
	FGameplayTag LayerID;
	
};

USTRUCT()
struct FHUDElementEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "UI", meta = (AssetBundles="Client"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	FGameplayTag SlotID;
};

UCLASS(MinimalAPI, meta = (DisplayName = "Add Widgets"))
class UGameFeatureAction_AddWidgets : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	
#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

	UPROPERTY(EditAnywhere, Category = "UI", meta=(TitleProperty = "{LayerID} -> {LayoutClass}"))
	TArray<FHUDLayoutRequest> Layout;
	
	UPROPERTY(EditAnywhere, Category = "UI", meta=(TitleProperty = "{LayerID} -> {LayoutClass}"))
	TArray<FHUDElementEntry> Widgets;

private:
	struct FPerActorData
	{
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;
		TArray<FUIExtensionHandle> ExtensionHandles;
	};

	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TMap<FObjectKey, FPerActorData> ActorData;
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

	void Reset(FPerContextData& ActiveData) const;
	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData) const;
};

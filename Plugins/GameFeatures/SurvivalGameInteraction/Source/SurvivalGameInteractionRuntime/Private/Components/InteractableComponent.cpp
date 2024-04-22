// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractableComponent.h"

UInteractableComponent::UInteractableComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInteractableComponent::HighlightActor()
{
	SetOwnerStaticMeshRenderCustomDepth(true);
}

void UInteractableComponent::UnHighlightActor()
{
	SetOwnerStaticMeshRenderCustomDepth(false);
}

void UInteractableComponent::SetOwnerStaticMeshRenderCustomDepth(const bool bValue) const
{
	const AActor* Owner = GetOwner();
	if (Owner == nullptr)
	{
		return;
	}

	TArray<UStaticMeshComponent*> StaticMeshComponents;
	Owner->GetComponents(UStaticMeshComponent::StaticClass(), StaticMeshComponents);
	for (UStaticMeshComponent* StaticMeshComp : StaticMeshComponents)
	{
		StaticMeshComp->SetRenderCustomDepth(bValue);
	}
}

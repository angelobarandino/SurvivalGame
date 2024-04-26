// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionLibrary.h"

#include "InteractionManagerComponent.h"

UInteractionManagerComponent* UInteractionLibrary::GetInteractionManager(AController* Controller)
{
	if (Controller)
	{
		return Controller->FindComponentByClass<UInteractionManagerComponent>();
	}

	return nullptr;
}


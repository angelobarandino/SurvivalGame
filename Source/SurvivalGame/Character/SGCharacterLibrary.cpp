// Fill out your copyright notice in the Description page of Project Settings.


#include "SGCharacterLibrary.h"

#include "GameFramework/Character.h"

const USkeletalMeshComponent* USGCharacterLibrary::GetCharacterSkeletalMesh(const AActor* Actor)
{
	if (const ACharacter* Character = Cast<ACharacter>(Actor))
	{
		return Character->GetMesh();
	}
	
	return nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTasks/AbilityTask_FindOrSpawnLootContainer.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Pickups/IPickupable.h"
#include "Pickups/ItemPickupContainer.h"


UAbilityTask_FindOrSpawnLootContainer::UAbilityTask_FindOrSpawnLootContainer(
	const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CollisionChannel = ECC_Visibility;
}

UAbilityTask_FindOrSpawnLootContainer* UAbilityTask_FindOrSpawnLootContainer::FindOrSpawnLootItemContainer(
	UGameplayAbility* OwningAbility, const UInventoryItemInstance* ItemInstance, const ECollisionChannel TraceChannel, const TSubclassOf<AItemPickupContainer> LootContainerClass)
{
	UAbilityTask_FindOrSpawnLootContainer* AbilityTask = NewAbilityTask<UAbilityTask_FindOrSpawnLootContainer>(OwningAbility);
	AbilityTask->CurrentItemInstance = ItemInstance;
	AbilityTask->CollisionChannel = TraceChannel;
	AbilityTask->ItemPickupContainerClass = LootContainerClass;
	return AbilityTask; 
}

void UAbilityTask_FindOrSpawnLootContainer::Activate()
{
	Super::Activate();

	if (!CurrentItemInstance)
	{
		EndTask();
	}
	
	FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent.Get(), true);
	
	CallOrAddReplicatedDelegate(EAbilityGenericReplicatedEvent::GameCustom1, FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnSuccessCallback));
	CallOrAddReplicatedDelegate(EAbilityGenericReplicatedEvent::GameCustom2, FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnFailedCallback));
	
	if (Ability->GetCurrentActorInfo()->IsNetAuthority())
	{
		APawn* Avatar = Cast<APawn>(GetAvatarActor());
		const FVector ActorLocation = Avatar->GetActorLocation();
		LootContainer = FindItemContainerAroundPlayer(ActorLocation);
		
		if (LootContainer == nullptr)
		{
			for (int Index = 0; Index < 10; ++Index)
			{
				LootContainer = TrySpawnNewItemContainerAtLocation(Avatar, GenerateRandomLocation(ActorLocation));
				if (LootContainer)
				{
					break;
				}
			}
		}
		
		if (IsValid(LootContainer))
		{
			AbilitySystemComponent->ClientSetReplicatedEvent(EAbilityGenericReplicatedEvent::GameCustom1, GetAbilitySpecHandle(), GetActivationPredictionKey());

			if (ShouldBroadcastAbilityTaskDelegates())
			{
				Success.Broadcast(LootContainer);
			}
		}
		else
		{
			AbilitySystemComponent->ClientSetReplicatedEvent(EAbilityGenericReplicatedEvent::GameCustom2, GetAbilitySpecHandle(), GetActivationPredictionKey());
	        
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				Failed.Broadcast(nullptr);
			}
		}
	}
}

void UAbilityTask_FindOrSpawnLootContainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, LootContainer);
}

AActor* UAbilityTask_FindOrSpawnLootContainer::FindItemContainerAroundPlayer(const FVector& Location) const
{
	const UWorld* World = GEngine->GetWorldFromContextObject(Ability, EGetWorldErrorMode::LogAndReturnNull);
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(300);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetAvatarActor());

	TArray<FHitResult> OutHitResults;
	if (World->SweepMultiByChannel(OutHitResults, Location, Location, FQuat::Identity, CollisionChannel, CollisionShape, QueryParams))
	{
		for (const FHitResult& HitResult : OutHitResults)
		{
			if (HitResult.bBlockingHit)
			{
				if (HitResult.GetActor() && HitResult.GetActor()->Implements<UPickupable>())
				{
					return HitResult.GetActor();
				}
			}
		}
	}

	return nullptr;
}

FVector UAbilityTask_FindOrSpawnLootContainer::GenerateRandomLocation(const FVector& CenterLocation) const
{
	const float RandomX = FMath::FRandRange(80.f, 150.f);
	const float RandomY = FMath::FRandRange(80.f, 150.f);
	const float PointInRaduis = FMath::FRandRange(0.f,
		UKismetMathLibrary::MultiplyByPi(2));

	const FVector RandomLocation(
		RandomX * FMath::Cos(PointInRaduis),
		RandomY * FMath::Sin(PointInRaduis),
		0);
	
	return RandomLocation + CenterLocation + FVector(0,0, 60.f);
}

AActor* UAbilityTask_FindOrSpawnLootContainer::TrySpawnNewItemContainerAtLocation(APawn* Instigator, const FVector& SpawnLocation) const
{
	UWorld* World = GetWorld();
	const FVector Start = SpawnLocation;
	const FVector End = SpawnLocation + (FVector::DownVector * 500);

	FHitResult OutHit;
	if (World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility))
	{
		FTransform SpawnTransform;
		SpawnTransform.SetRotation(UKismetMathLibrary::MakeRotFromZ(OutHit.ImpactNormal).Quaternion());
		SpawnTransform.SetLocation(OutHit.ImpactPoint);
		
		if (AItemPickupContainer* ItemContainer = World->SpawnActorDeferred<AItemPickupContainer>(
			ItemPickupContainerClass,
			SpawnTransform,
			nullptr,
			Instigator,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding))
		{
			ItemContainer->MaxInventorySize = 30;
			ItemContainer->FinishSpawning(SpawnTransform);
			return ItemContainer;
		}
	}
	
	return nullptr;
}

void UAbilityTask_FindOrSpawnLootContainer::OnSuccessCallback()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::GameCustom1, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		Success.Broadcast(LootContainer);
	}
}

void UAbilityTask_FindOrSpawnLootContainer::OnFailedCallback()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::GameCustom2, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		Failed.Broadcast(nullptr);
	}
}

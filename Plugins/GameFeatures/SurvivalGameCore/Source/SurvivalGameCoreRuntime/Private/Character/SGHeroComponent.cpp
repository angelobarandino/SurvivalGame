// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SGHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SurvivalGame/SGGameplayTags.h"
#include "SurvivalGame/AbilitySystem/SGAbilitySystemComponent.h"
#include "SurvivalGame/Character/SGPawnManagerComponent.h"
#include "SurvivalGame/Input/SGInputComponent.h"
#include "SurvivalGame/Player/SGPlayerController.h"
#include "SurvivalGame/Player/SGPlayerState.h"

const FName USGHeroComponent::NAME_ActorFeatureName("Hero");

USGHeroComponent::USGHeroComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USGHeroComponent::OnRegister()
{
	Super::OnRegister();

	RegisterInitStateFeature();
}

void USGHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const USpringArmComponent* SpringArm = Pawn->FindComponentByClass<USpringArmComponent>())
		{
			TargetZoomDistance = SpringArm->TargetArmLength;
		}
	}

	BindOnActorInitStateChanged(USGPawnManagerComponent::NAME_ActorFeatureName, FGameplayTag(), false);
	ensure(TryToChangeInitState(SGGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void USGHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void USGHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = {
		SGGameplayTags::InitState_Spawned,
		SGGameplayTags::InitState_DataAvailable,
		SGGameplayTags::InitState_DataInitialized,
		SGGameplayTags::InitState_GameplayReady
	};

	ContinueInitStateChain(StateChain);
}

bool USGHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* OwningPawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() &&  DesiredState == SGGameplayTags::InitState_Spawned)
	{
		if (OwningPawn)
		{
			return true;
		}
	}
	else if (CurrentState == SGGameplayTags::InitState_Spawned && DesiredState == SGGameplayTags::InitState_DataAvailable)
	{
		if (GetPlayerState<ASGPlayerState>() == nullptr)
		{
			return false;
		}

		if (OwningPawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			const AController* Controller = GetController<AController>();
			if (!ensure(Controller && Controller->PlayerState))
			{
				return false;
			}
		}

		if (OwningPawn->IsLocallyControlled())
		{
			const ASGPlayerController* PC = GetController<ASGPlayerController>();
			if (!OwningPawn->InputComponent || !PC || !PC->GetLocalPlayer())
			{
				return false;
			}
		}
		
		return true;
	}
	else if (CurrentState == SGGameplayTags::InitState_DataAvailable && DesiredState == SGGameplayTags::InitState_DataInitialized)
	{
		return GetPlayerState<ASGPlayerState>() && Manager->HasFeatureReachedInitState(OwningPawn, USGPawnManagerComponent::NAME_ActorFeatureName, SGGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == SGGameplayTags::InitState_DataInitialized && DesiredState == SGGameplayTags::InitState_GameplayReady)
	{
		return true;
	}
	
	return false;
}

void USGHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == SGGameplayTags::InitState_DataAvailable && DesiredState == SGGameplayTags::InitState_DataInitialized)
	{
		const APawn* OwningPawn = GetPawn<APawn>();
		const ASGPlayerState* PlayerState = GetPlayerState<ASGPlayerState>();
		if (!ensure(OwningPawn && PlayerState))
		{
			return;
		}
		
		const APlayerController* PC = GetController<APlayerController>();
		if (PC == nullptr)
		{
			return;
		}
		
		if (USGPawnManagerComponent* PawnManager = OwningPawn->FindComponentByClass<USGPawnManagerComponent>())
		{
			PawnManager->SetAbilitySystemComponent(PlayerState->GetAbilitySystemComponent());
			
			if (OwningPawn->InputComponent)
			{
				InitializeDefaultPlayerInput(OwningPawn->InputComponent);
				
				PawnManager->ListenOnAdditionalInputConfig(FOnBindAdditionalInputConfig::FDelegate::CreateUObject(this, &ThisClass::BindAdditionalInputConfig));
				PawnManager->ListenOnRemoveInputBindings(FOnRemoveInputBindings::FDelegate::CreateUObject(this, &ThisClass::RemoveInputBindings));
				
				UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(OwningPawn), USGPawnManagerComponent::NAME_BindInputsReady);
				UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), USGPawnManagerComponent::NAME_BindInputsReady);
			}
		}
	}
}

void USGHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == USGPawnManagerComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == SGGameplayTags::InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

void USGHeroComponent::InitializeDefaultPlayerInput(UInputComponent* PlayerInputComponent)
{
	if (bDefaultPlayerInputInitialized)
	{
		return;
	}
	
	check(PlayerInputComponent);
	check(DefaultInputConfig);
	check(DefaultMappingContext);
	
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	if (USGInputComponent* SGInput = Cast<USGInputComponent>(PlayerInputComponent))
	{
		SGInput->BindNativeAction(DefaultInputConfig, SGGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		SGInput->BindNativeAction(DefaultInputConfig, SGGameplayTags::InputTag_Rotate, ETriggerEvent::Triggered, this, &ThisClass::Input_Rotate);
		SGInput->BindNativeAction(DefaultInputConfig, SGGameplayTags::InputTag_Zoom, ETriggerEvent::Triggered, this, &ThisClass::Input_Zoom);

		if (ensure(!bDefaultPlayerInputInitialized))
		{
			bDefaultPlayerInputInitialized = true;
		}
	}
}

void USGHeroComponent::BindAdditionalInputConfig(const USGInputConfig* InputConfig)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	USGInputComponent* Input = Pawn->FindComponentByClass<USGInputComponent>();
	if (ensureMsgf(Input, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to USGInputComponent.")))
	{
		Input->BindAbilityAction(InputConfig, this, &ThisClass::Input_AbilityPressed, &ThisClass::Input_AbilityReleased, AbilityInputBindHandles);
	}
}

void USGHeroComponent::RemoveInputBindings()
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	//@TODO: Pawn->InputComponent is NULL here.
	if (USGInputComponent* Input = Cast<USGInputComponent>(Pawn->InputComponent))
	{
		for (const uint32& InputHandle : AbilityInputBindHandles)
		{
			Input->RemoveBindingByHandle(InputHandle);
		}
	}
}

void USGHeroComponent::Input_Move(const FInputActionValue& Value)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		if (const AController* Controller = Pawn->GetController<AController>())
		{
			const FVector2D MoveVector = Value.Get<FVector2D>();
			const FRotator MoveRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

			if (MoveVector.X != 0.f)
			{
				const FVector RightMovement = FRotationMatrix(MoveRotation).GetUnitAxis(EAxis::Y);
				Pawn->AddMovementInput(RightMovement, MoveVector.X, false);	
			}

			if (MoveVector.Y != 0.f)
			{
				const FVector ForwardMovement = FRotationMatrix(MoveRotation).GetUnitAxis(EAxis::X);
				Pawn->AddMovementInput(ForwardMovement, MoveVector.Y, false);
			}
		}
	}
}

void USGHeroComponent::Input_Rotate(const FInputActionValue& Value)
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		const FVector2D LookVector = Value.Get<FVector2D>();
		Pawn->AddControllerYawInput(LookVector.X);
		Pawn->AddControllerPitchInput(LookVector.Y);
	}
}

void USGHeroComponent::Input_Zoom(const FInputActionValue& Value)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const USpringArmComponent* SpringArm = Pawn->FindComponentByClass<USpringArmComponent>())
		{
			const float Direction = Value.GetMagnitude();
			const float CurrentZoomDistance = SpringArm->TargetArmLength;
			const float NewZoomDistance = CurrentZoomDistance + ZoomStepSize * Direction;
			TargetZoomDistance = FMath::Clamp(NewZoomDistance, ZoomMin, ZoomMax);

			GetWorldTimerManager().ClearTimer(ZoomTimerHandle);
			GetWorldTimerManager().SetTimer(ZoomTimerHandle, this, &ThisClass::OnZoom, 0.01f, true);
		}
	}
}

void USGHeroComponent::OnZoom()
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (USpringArmComponent* SpringArm = Pawn->FindComponentByClass<USpringArmComponent>())
		{
			if (FMath::IsNearlyEqual(SpringArm->TargetArmLength, TargetZoomDistance, 0.1f))
			{
				GetWorldTimerManager().ClearTimer(ZoomTimerHandle);
				return;
			}

			const float DeltaTime = GetWorld()->DeltaRealTimeSeconds;
			SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, TargetZoomDistance, DeltaTime, ZoomSpeed);
		}
	}
}

void USGHeroComponent::Input_AbilityPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const USGPawnManagerComponent* PawnManager = Pawn->FindComponentByClass<USGPawnManagerComponent>())
		{
			if (USGAbilitySystemComponent* ASC = PawnManager->GetAbilitySystemComponent())
			{
				ASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void USGHeroComponent::Input_AbilityReleased(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const USGPawnManagerComponent* PawnManager = Pawn->FindComponentByClass<USGPawnManagerComponent>())
		{
			if (USGAbilitySystemComponent* ASC = PawnManager->GetAbilitySystemComponent())
			{
				ASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
}

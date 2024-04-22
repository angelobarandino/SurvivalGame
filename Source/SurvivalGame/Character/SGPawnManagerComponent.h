// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "SurvivalGame/Player/SGPlayerState.h"
#include "SGPawnManagerComponent.generated.h"

class UAbilitySystemComponent;
class USGInputConfig;
class USGAbilitySystemComponent;
class USGCharacterConfig;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBindAdditionalInputConfig, const USGInputConfig*);
DECLARE_MULTICAST_DELEGATE(FOnRemoveInputBindings);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALGAME_API USGPawnManagerComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	USGPawnManagerComponent(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing=OnRep_CharacterData)
	TObjectPtr<const USGCharacterConfig> CharacterData;
	
protected:
	UFUNCTION()
	void OnRep_CharacterData();
	
	virtual void OnRegister() override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USGAbilitySystemComponent> AbilitySystemComponent;
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateInitializationState();
	void SetCharacterData(const USGCharacterConfig* InCharacterData);
	
	/** The name of this overall feature */
	static const FName NAME_ActorFeatureName;
	static const FName NAME_BindInputsReady;
	
	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual void CheckDefaultInitialization() override;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	//~ End IGameFrameworkInitStateInterface interface
	
	void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent);
	USGAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

	void BindAdditionalInputConfig(const USGInputConfig* InputConfig) const;
	void ListenOnAdditionalInputConfig(FOnBindAdditionalInputConfig::FDelegate&& Delegate);
	
	void RemoveInputBindings() const;
	void ListenOnRemoveInputBindings(FOnRemoveInputBindings::FDelegate&& Delegate);

private:
	FOnBindAdditionalInputConfig OnBindAdditionalInputConfig;
	FOnRemoveInputBindings OnRemoveInputBindings;
};

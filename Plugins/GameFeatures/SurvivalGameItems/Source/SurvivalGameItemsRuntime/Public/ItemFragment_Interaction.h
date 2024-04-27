#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "ItemFragment_Interaction.generated.h"

struct FInteractOption;

UCLASS(BlueprintType, meta=(DisplayName = "Interaction"))
class SURVIVALGAMEITEMSRUNTIME_API UItemFragment_Interaction : public UItemFragment
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Interaction")
	FName DisplayName;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSoftClassPtr<UUserWidget> InteractWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = "Interaction")
	TArray<FInteractOption> InteractOptions;
	
};
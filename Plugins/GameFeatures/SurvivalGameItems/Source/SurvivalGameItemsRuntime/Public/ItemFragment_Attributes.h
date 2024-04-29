#pragma once

#include "CoreMinimal.h"
#include "ItemFragment.h"
#include "ItemFragment_Attributes.generated.h"

UCLASS(meta=(DisplayName = "Attributes"))
class UItemFragment_Attributes : public UItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes", meta=(Tooltip="Strength of the item's effect. (e.g., Healing amount, Damage dealt, Buff magnitude)"))
	float EffectMagnitude = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes", meta=(MultiLine=true, Tooltip="Text description of the item's effect. (e.g., Heals X health points, Increases Damage by Y%, Grants Z buff)"))
	FText EffectDescription;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes", meta=(Tooltip="Numeric value for remaining durability (if applicable)"))
	float Durability = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes", meta=(Tooltip="Measures how long the item can be used before needing to recharge. (Applicable to consumables or abilities)"))
	float Cooldown = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes", meta=(Tooltip="Minimum character level required to use this item effectively."))
	float LevelRequirement = 0.f;
	
};

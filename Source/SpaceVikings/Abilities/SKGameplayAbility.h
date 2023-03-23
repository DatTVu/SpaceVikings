#pragma once

#include "../SpaceVikings.h"
#include "Abilities/GameplayAbility.h"
#include "SKGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class SPACEVIKINGS_API USKGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
		USKGameplayAbility();
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input ID")
		ESK_AbilityInput AbilityInputID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input ID")
		ESK_AbilityInput AbilityID;

};
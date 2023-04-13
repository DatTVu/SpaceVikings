#pragma once

#include "../SpaceVikings.h"
#include "Abilities/GameplayAbility.h"
#include "SKAbilityTypes.h"
#include "SKGameplayAbility.generated.h"

UCLASS()
class SPACEVIKINGS_API USKGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	USKGameplayAbility();

	/** Map of gameplay tags to gameplay effect containers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FSKGameplayEffectContainer> EffectContainerMap;

	/** Make gameplay effect container spec to be applied later, using the passed in container */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FSKGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FSKGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** Search for and make a gameplay effect container spec to be applied later, from the EffectContainerMap */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FSKGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	/** Applies a gameplay effect container spec that was previously created */
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FSKGameplayEffectContainerSpec& ContainerSpec);

	/** Applies a gameplay effect container, by creating and then applying the spec */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input ID")
	ESK_AbilityInput AbilityInputID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input ID")
	ESK_AbilityInput AbilityID;
};
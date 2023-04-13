// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../SpaceVikings.h"
#include "AbilitySystemComponent.h"
#include "SKAbilitySystemComponent.generated.h"

class USKGameplayAbility;
UCLASS()
class SPACEVIKINGS_API USKAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	// Constructors and overrides
	USKAbilitySystemComponent();

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<USKGameplayAbility*>& ActiveAbilities);

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static USKAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);
	
};

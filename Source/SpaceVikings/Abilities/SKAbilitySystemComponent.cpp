// Fill out your copyright notice in the Description page of Project Settings.


#include "SKAbilitySystemComponent.h"
#include "SKGameplayAbility.h"
#include "../Characters/SKCharacterBase.h"
#include "AbilitySystemGlobals.h"

USKAbilitySystemComponent::USKAbilitySystemComponent() {
	
}

void USKAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<USKGameplayAbility*>& ActiveAbilities) {
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<USKGameplayAbility>(ActiveAbility));
		}
	}
}

USKAbilitySystemComponent* USKAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent) {
	return Cast<USKAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
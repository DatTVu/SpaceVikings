// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SKPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEVIKINGS_API ASKPlayerController : public APlayerController
{
	GENERATED_BODY()
public: 
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnManaChanged(const float currentMana, const float maxMana);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnExhaustion();
};

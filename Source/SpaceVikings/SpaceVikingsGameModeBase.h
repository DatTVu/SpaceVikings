// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceVikingsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEVIKINGS_API ASpaceVikingsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;
	
};

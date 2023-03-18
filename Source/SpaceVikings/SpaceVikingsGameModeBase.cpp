// Copyright Epic Games, Inc. All Rights Reserved.


#include "SpaceVikingsGameModeBase.h"

void ASpaceVikingsGameModeBase::StartPlay() {
	Super::StartPlay();

	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Welcome to Space Viking!"));
}
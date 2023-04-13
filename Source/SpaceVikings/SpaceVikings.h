// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"

UENUM()
enum class ESK_AbilityInput : uint8 {
	None, 
	Confirm, 
	Cancel,
	Sprint,
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../SpaceVikings.h"
#include "SKCharacterBase.h"
#include "SKEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SPACEVIKINGS_API ASKEnemyCharacter : public ASKCharacterBase
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ASKEnemyCharacter();

	bool IsActive();

	void SetActive(bool isActive);

	void SetLifeSpan(float lifeSpan);

	int GetPoolIndex();

	void SetPoolIndex(int idx);


protected:
	bool BIsActive = false;

	float LifeSpan;

	int PoolIndex = 0;

	FTimerHandle LifeSpanTimer;
};

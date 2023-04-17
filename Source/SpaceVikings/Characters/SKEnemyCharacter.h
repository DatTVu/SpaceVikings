// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../SpaceVikings.h"
#include "SKCharacterBase.h"
#include "SKEnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledEnemyDespawn, ASKEnemyCharacter*, PooledEnemy);

UCLASS()
class SPACEVIKINGS_API ASKEnemyCharacter : public ASKCharacterBase
{
	GENERATED_BODY()
public:
	ASKEnemyCharacter();

	FOnPooledEnemyDespawn OnPooledEnemyDespawn;

	UFUNCTION(BlueprintCallable, Category = "Pooled Actor")
	void Deactivate();

	// Properties that enable the object to be pooled
	bool IsActive();

	void SetActive(bool isActive);

	int GetPoolIndex();

	void SetPoolIndex(int idx);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaticMesh")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoxCollision")
	class UBoxComponent* BoxCollision;

protected:
	bool m_IsActive = false;
	int m_PoolIndex = 0;
};

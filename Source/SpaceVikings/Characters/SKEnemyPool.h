// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SKEnemyPool.generated.h"

class ASKEnemyCharacter;
UCLASS()
class SPACEVIKINGS_API ASKEnemyPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASKEnemyPool();

	UFUNCTION()
	void OnPooledEnemyDespawn(ASKEnemyCharacter* PooledEnemy);

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	int PoolSize = 0;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	float LineStartXPos = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	float MaxLineStartXPos = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	float LineStartYPos = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	int EnemyPerLine = 0;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	float XStride = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	float YStride = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	float MoveSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	float MoveSpeedIncrement = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	TArray<TSubclassOf<ASKEnemyCharacter>> EnemyCharacterClassVec;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<ASKEnemyCharacter*> EnemyActorPool;
	TArray<int> SpawnedEnemyActorIndex;
	void ResetEnemies();

private:
	TAtomic<bool> m_IsSpawning = true;
	float m_direction;
	bool m_bFlippedDirection;
	bool m_bMoveDown;
	float m_shootTime;
	float m_shootTimer;
	bool m_canShoot = false;
	int m_enemyAliveCount = 0;
	int m_enemyClassCnt = 0;
	int m_enemyCntPerClass = 0;
	int m_rowPerClass = 0;
	int m_maxRowIdx = 0;
	TArray<TArray<int>> m_aliveEnemyPos;
};
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

	UFUNCTION(BlueprintCallable, Category = "Enemy Pool")
	ASKEnemyCharacter* SpawnPooledEnemies();

	UFUNCTION()
	void OnPooledEnemyDespawn(ASKEnemyCharacter* PooledEnemy);

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	TSubclassOf<ASKEnemyCharacter> PooledEnemySubClass;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	int PoolSize = 10;

	UPROPERTY(EditAnywhere, Category = "Enemy Pool")
	float ActorLifeSpan;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<ASKEnemyCharacter*> EnemyActorPool;
	TArray<int> SpawnedEnemyActorIndex;

private:
	float m_direction;
	bool m_bFlippedDirection;
	bool m_bMoveDown;
	float m_shootTime;
	float m_shootTimer;
	bool m_canShoot = false;
};
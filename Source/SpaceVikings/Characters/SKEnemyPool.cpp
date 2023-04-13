// Fill out your copyright notice in the Description page of Project Settings.


#include "SKEnemyPool.h"
#include "SKEnemyCharacter.h"

// Sets default values
ASKEnemyPool::ASKEnemyPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ASKEnemyCharacter* ASKEnemyPool::SpawnPooledEnemies()
{
	return nullptr;
}

void ASKEnemyPool::OnPooledEnemyDespawn(ASKEnemyCharacter* PooledActor)
{
}

// Called when the game starts or when spawned
void ASKEnemyPool::BeginPlay()
{
	Super::BeginPlay();
	if (PooledEnemySubClass != nullptr) {
		UWorld* const World = GetWorld();
		float XStride = 90.0f;
		float YStride = 60.0f;
		if (World != nullptr) {
			for (int i = 0; i < 5; ++i) {
				for (int j = -3; j < 3; ++j) {
					ASKEnemyCharacter* PooledEnemy = World->SpawnActor<ASKEnemyCharacter>(PooledEnemySubClass, FVector(XStride *i, 0 - YStride *j, 0), FRotator(0, 0, 0));
					if (PooledEnemy != nullptr) {
						PooledEnemy->SetActive(true);
						//PooledEnemy->SetPoolIndex(i+j);
						//PooledEnemy->OnPooledProjectileDespawn.AddDynamic(this, &USKProjectilePool::OnPooledActorDespawn);
						EnemyActorPool.Add(PooledEnemy);
					}
				}
			}
		}
	}
}

// Called every frame
void ASKEnemyPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


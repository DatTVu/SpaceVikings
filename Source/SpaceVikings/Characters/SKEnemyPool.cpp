// Fill out your copyright notice in the Description page of Project Settings.


#include "SKEnemyPool.h"
#include "SKEnemyCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ASKEnemyPool::ASKEnemyPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_direction = 1.0f;
	m_bFlippedDirection = false;
	m_bMoveDown = false;
    m_shootTime = 2.0f;
    m_shootTimer = 0.0f;
}

ASKEnemyCharacter* ASKEnemyPool::SpawnPooledEnemies()
{
	return nullptr;
}


void ASKEnemyPool::OnPooledEnemyDespawn(ASKEnemyCharacter* PooledEnemy)
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
					ASKEnemyCharacter* PooledEnemy = World->SpawnActor<ASKEnemyCharacter>(PooledEnemySubClass, FVector(XStride *i, YStride *j, 0.0), FRotator(0, 180, 0));
					if (PooledEnemy != nullptr) {
                        if (i == 0) {
                            PooledEnemy->SetActive(true);
                        }
						//PooledEnemy->OnPooledEnemyDespawn.AddDynamic(this, &ASKEnemyPool::OnPooledEnemyDespawn);
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
    m_shootTimer += DeltaTime;
    if (m_shootTimer >= m_shootTime) {
        m_shootTimer -= m_shootTime;
        m_canShoot = true;
    }

    for (auto& it : EnemyActorPool) {
        FVector movement;
        float speed = 2.0f;
        if (!m_bMoveDown) {
            movement = FVector(0, m_direction * speed, 0);
        }
        else {
            movement = FVector(-speed * 2, 0, 0);
        }

        it->AddActorWorldOffset(movement);
        if (it->IsActive() && m_canShoot) {
            it->Fire();
        }

        /* If we hit the border with one of our ships this movement, change direction
         * and move down in the next movement */
        if (!m_bMoveDown && FMath::Abs(it->GetActorLocation().Y) >= 450) {
            m_bFlippedDirection = true;
        }
    }

    if (m_bFlippedDirection) {
        /* Flip the direction */
        m_direction = -m_direction;
        m_bFlippedDirection = false;
        /* If we've flipped direction, we should move down first */
        m_bMoveDown = true;
    }
    else {
        m_bMoveDown = false;
    }
    m_canShoot = false;
}


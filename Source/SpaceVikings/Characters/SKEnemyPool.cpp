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


void ASKEnemyPool::OnPooledEnemyDespawn(ASKEnemyCharacter* PooledEnemy)
{
    m_IsSpawning = true;
    PooledEnemy->TeleportTo(FVector(10000, 10000, -10000), FRotator(0, 0, 0));
    --m_enemyAliveCount;
    MoveSpeed += MoveSpeedIncrement;
    int rowIdx = PooledEnemy->GetRowIndex();
    int colIdx = PooledEnemy->GetColIndex();
    m_aliveEnemyPos[colIdx][rowIdx] = 0;

    int i = m_aliveEnemyPos[colIdx].Num() - 1;
    while (i >= 0 && m_aliveEnemyPos[colIdx][i] == 0) {
        --i;
    }

    if (i >= 0 && m_aliveEnemyPos[colIdx][i] == 1) {
        EnemyActorPool[i * EnemyPerLine + colIdx]->SetActive(true);
    }
    
    if (m_enemyAliveCount <= 0) {
        m_enemyAliveCount = PoolSize;
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("NEXT LEVEL!!!"));
        LineStartXPos -= XStride;
        if (LineStartXPos < MaxLineStartXPos) {
            LineStartXPos = MaxLineStartXPos;
        }   
        ResetEnemies();
    }
    m_IsSpawning = false;
}

// Called when the game starts or when spawned
void ASKEnemyPool::BeginPlay()
{
	Super::BeginPlay();
    m_enemyAliveCount = PoolSize;
    EnemyActorPool.Reserve(PoolSize);
    UWorld* const World = GetWorld();
    m_maxRowIdx = (PoolSize - EnemyPerLine) / EnemyPerLine;
    m_enemyClassCnt = EnemyCharacterClassVec.Num();
    m_enemyCntPerClass = PoolSize / m_enemyClassCnt;
    m_rowPerClass = m_enemyCntPerClass / EnemyPerLine;
    m_aliveEnemyPos.Reserve(EnemyPerLine);

    for (int i = 0; i < EnemyPerLine; ++i) {
        TArray<int> vec;
        vec.Init(1, PoolSize / EnemyPerLine);
        m_aliveEnemyPos.Emplace(vec);
    }

    m_IsSpawning = true;
	if (World != nullptr) {
        for (int i = 0; i < m_enemyClassCnt; ++i) {
            for (int j = 0; j < m_enemyCntPerClass; ++j) {
                int xIdx = j / EnemyPerLine + i * m_rowPerClass;
                int yIdx = j % EnemyPerLine;
                ASKEnemyCharacter* PooledEnemy = World->SpawnActor<ASKEnemyCharacter>(EnemyCharacterClassVec[i], 
                    FVector(LineStartXPos - XStride * xIdx, LineStartYPos + YStride * yIdx, 0.0), FRotator(0, 180, 0));
                if (PooledEnemy != nullptr) {
                    if (xIdx >= m_maxRowIdx) {
                        PooledEnemy->SetActive(true);
                    }
                    PooledEnemy->OnPooledEnemyDespawn.AddDynamic(this, &ASKEnemyPool::OnPooledEnemyDespawn);
                    PooledEnemy->SetCanMove(true);
                    PooledEnemy->SetRowIndex(xIdx);
                    PooledEnemy->SetColIndex(yIdx);
                    m_aliveEnemyPos[yIdx][xIdx] = 1;
                    EnemyActorPool.Add(PooledEnemy);
                }
            }
        }
	}
    m_IsSpawning = false;
}

void ASKEnemyPool::ResetEnemies()
{
    m_IsSpawning = true;
    for (int i = 0; i < PoolSize; ++i) {
        ASKEnemyCharacter* PooledEnemy = EnemyActorPool[i];
        if (PooledEnemy != nullptr) {
            int xIdx = PooledEnemy->GetRowIndex();
            int yIdx = PooledEnemy->GetColIndex();
            if (xIdx >= m_maxRowIdx) {
                PooledEnemy->SetActive(true);
            }
            m_aliveEnemyPos[yIdx][xIdx] = 1;
            PooledEnemy->TeleportTo(FVector(LineStartXPos - XStride * xIdx, LineStartYPos + YStride * yIdx, 80.0), FRotator(0, 180, 0));
            PooledEnemy->SetCanMove(true);
        }
    }
    m_IsSpawning = false;
}

// Called every frame
void ASKEnemyPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (!m_IsSpawning) {
        m_shootTimer += DeltaTime;
        if (m_shootTimer >= m_shootTime) {
            m_shootTimer -= m_shootTime;
            m_canShoot = true;
        }

        for (auto& it : EnemyActorPool) {
            FVector movement;
            if (!m_bMoveDown) {
                movement = FVector(0, m_direction * MoveSpeed, 0);
            }
            else {
                movement = FVector(-MoveSpeed * 2, 0, 0);
            }

            if (it->CanMove()) {
                it->AddActorWorldOffset(movement);
                /* If we hit the border with one of our ships this movement, change direction
                * and move down in the next movement */
                if (!m_bMoveDown && FMath::Abs(it->GetActorLocation().Y) >= 600) {
                    m_bFlippedDirection = true;
                }
            }

            if (it->IsActive() && m_canShoot) {
                it->Fire();
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
}


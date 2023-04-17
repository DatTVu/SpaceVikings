#include "SKProjectilePool.h"
#include "SKProjectileBase.h"

USKProjectilePool::USKProjectilePool() {
	//ProjectileSubClass = ASKProjectileBase;
}

void USKProjectilePool::BeginPlay() {
	Super::BeginPlay();
	if (PooledObjectSubClass != nullptr) {
		UWorld* const World = GetWorld();

		if (World != nullptr) {
			for (int i = 0; i < PoolSize; ++i) {
				ASKProjectileBase* PooledProjectile = World->SpawnActor<ASKProjectileBase>(PooledObjectSubClass, FVector(100000, 100000, -100000), FRotator(0, 0, 0));
				if (PooledProjectile != nullptr) {
					PooledProjectile->SetActive(false);
					PooledProjectile->SetPoolIndex(i);
					PooledProjectile->OnPooledProjectileDespawn.AddDynamic(this, &USKProjectilePool::OnPooledActorDespawn);
					ActorPool.Add(PooledProjectile);
				}
			}
		}
	}
}

ASKProjectileBase* USKProjectilePool::SpawnPooledProjectile() {
	for (ASKProjectileBase* PooledProjectile : ActorPool) {
		if (PooledProjectile != nullptr && !PooledProjectile->IsActive()) {
			PooledProjectile->TeleportTo(FVector(100000, 100000, -100000), FRotator(0, 0, 0));
			PooledProjectile->SetLifeSpan(ActorLifeSpan);
			PooledProjectile->SetActive(true);
			SpawnedActorIndex.Add(PooledProjectile->GetPoolIndex());
			return PooledProjectile;
		}
	}

	if (SpawnedActorIndex.Num() > 0) {
		int PooledIndex = SpawnedActorIndex[0];
		SpawnedActorIndex.Remove(PooledIndex);
		ASKProjectileBase* PooledProjectile = ActorPool[PooledIndex];
		if (PooledProjectile != nullptr) {
			PooledProjectile->TeleportTo(FVector(100000, 100000, -100000), FRotator(0, 0, 0));
			PooledProjectile->SetLifeSpan(ActorLifeSpan);
			PooledProjectile->SetActive(true);
			SpawnedActorIndex.Add(PooledProjectile->GetPoolIndex());
			return PooledProjectile;
		}
	}

	return nullptr;
}

void USKProjectilePool::OnPooledActorDespawn(ASKProjectileBase* PooledActor) {
	PooledActor->TeleportTo(FVector(100000, 100000, -100000), FRotator(0, 0, 0));
	SpawnedActorIndex.Remove(PooledActor->GetPoolIndex());
}



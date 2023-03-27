// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPooledActorBase.h"

// Sets default values
ASKPooledActorBase::ASKPooledActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASKPooledActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASKPooledActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Properties that enable the object to be pooled
void ASKPooledActorBase::SetActive(bool isActive) {
	BIsActive = isActive;
	SetActorHiddenInGame(!isActive);
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &ASKPooledActorBase::Deactivate, LifeSpan, false);
}

void ASKPooledActorBase::SetLifeSpan(float lifeSpan) {
	LifeSpan = lifeSpan;
}

void ASKPooledActorBase::SetPoolIndex(int idx) {
	PoolIndex = idx;
}

int ASKPooledActorBase::GetPoolIndex() {
	return PoolIndex;
}

bool ASKPooledActorBase::IsActive() {
	return BIsActive;
}

void ASKPooledActorBase::Deactivate() {
	SetActive(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	OnPooledActorDespawn.Broadcast(this);
}


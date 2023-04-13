// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../SpaceVikings.h"
#include "GameFramework/Actor.h"
#include "SKProjectilePool.generated.h"

class ASKProjectileBase;

UCLASS()
class SPACEVIKINGS_API USKProjectilePool : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USKProjectilePool();

	UFUNCTION(BlueprintCallable, Category = "Projectile Pool")
	ASKProjectileBase* SpawnPooledProjectile();

	UFUNCTION()
	void OnPooledActorDespawn(ASKProjectileBase* PooledActor);

	UPROPERTY(EditAnywhere, Category = "Projectile Pool")
	TSubclassOf<ASKProjectileBase> PooledObjectSubClass;

	UPROPERTY(EditAnywhere, Category = "Projectile Pool")
	int PoolSize = 10;

	UPROPERTY(EditAnywhere, Category = "Projectile Pool")
	float ActorLifeSpan;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<ASKProjectileBase*> ActorPool;
	TArray<int> SpawnedActorIndex;
};

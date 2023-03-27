#pragma once

#include "../SpaceVikings.h"
#include "GameFramework/Actor.h"
#include "SKPooledActorBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledActorDespawn, ASKPooledActorBase*, PoolActor);

UCLASS()
class SPACEVIKINGS_API ASKPooledActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASKPooledActorBase();

	FOnPooledActorDespawn OnPooledActorDespawn;

	UFUNCTION(BlueprintCallable, Category = "Pooled Projectile")
		void Deactivate();

	// Properties that enable the object to be pooled
	bool IsActive();

	void SetActive(bool isActive);

	void SetLifeSpan(float lifeSpan);

	int GetPoolIndex();

	void SetPoolIndex(int idx);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	bool BIsActive = false;
	float LifeSpan = 0.0f;
	int PoolIndex = 0;
	FTimerHandle LifeSpanTimer;

};

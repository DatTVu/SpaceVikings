#pragma once

#include "../SpaceVikings.h"
#include "GameFramework/Actor.h"
#include "SKProjectileBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledProjectileDespawn, ASKProjectileBase*, PooledProjectile);

UCLASS()
class SPACEVIKINGS_API ASKProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASKProjectileBase();

	FOnPooledProjectileDespawn OnPooledProjectileDespawn;

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

	// Function that is called when the projectile hits something.
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Sphere collision component.
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UStaticMeshComponent* ProjectileMeshComponent;

	// Projectile movement component
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	//Function to initialize the projectile's velocity in the shoot direction
	void FireInDirection(const FVector& shootDirection);

protected:
	bool BIsActive = false;
	float ProjectileLifeSpan;
	int PoolIndex = 0;
	FTimerHandle LifeSpanTimer;
};

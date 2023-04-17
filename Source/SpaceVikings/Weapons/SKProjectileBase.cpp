// Fill out your copyright notice in the Description page of Project Settings.


#include "SKProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASKProjectileBase::ASKProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!CollisionComponent) {
		//Use a sphere as simple collision component
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision profile name to "Projectile".
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		// Event called when component hits something.
		CollisionComponent->OnComponentHit.AddDynamic(this, &ASKProjectileBase::OnHit);
		//Set the sphere's collision radius
		//TO-DO: remove magic number here
		CollisionComponent->InitSphereRadius(15.0f);
		//Set the root component to the collision component;
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent) {
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		//TO-DO: remove magic number here
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.0f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!ProjectileMeshComponent) {
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Assets/Sphere.Sphere"));
		if (Mesh.Succeeded()) {
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
		ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
		ProjectileMeshComponent->SetupAttachment(CollisionComponent);
	}
}

// Called when the game starts or when spawned
void ASKProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ASKProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASKProjectileBase::FireInDirection(const FVector& ShootDirection) {
	ProjectileMovementComponent->SetUpdatedComponent(GetRootComponent());
	ProjectileMovementComponent->Velocity = FVector(1.0, 0.0, 0.0) * 500.0f;
}

// Function that is called when the projectile hits something.
void ASKProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
 	Deactivate();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("HIT!!!"));
}

// Properties that enable the object to be pooled
void ASKProjectileBase::SetActive(bool isActive) {
	BIsActive = isActive;
	SetActorHiddenInGame(!isActive);
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &ASKProjectileBase::Deactivate, ProjectileLifeSpan, false);
}

void ASKProjectileBase::SetLifeSpan(float lifeSpan) {
	ProjectileLifeSpan = lifeSpan;
}

void ASKProjectileBase::SetPoolIndex(int idx) {
	PoolIndex = idx;
}

int ASKProjectileBase::GetPoolIndex() {
	return PoolIndex;
}

bool ASKProjectileBase::IsActive() {
	return BIsActive;
}

void ASKProjectileBase::Deactivate() {
	SetActive(false);
	ProjectileMovementComponent->Velocity.X = 0;
	ProjectileMovementComponent->Velocity.Y = 0;
	ProjectileMovementComponent->Velocity.Z = 0;
	GetWorldTimerManager().ClearAllTimersForObject(this);
	OnPooledProjectileDespawn.Broadcast(this);
}

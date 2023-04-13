// Fill out your copyright notice in the Description page of Project Settings.


#include "SKProjectileBase.h"

ASKProjectileBase::ASKProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent) {
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
		CollisionComponent->InitSphereRadius(10.0f);
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
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!ProjectileMeshComponent) {
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Game/Assets/Sphere.Sphere"));
		if (Mesh.Succeeded()) {
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
		static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Game/Assets/SphereMaterial.SphereMaterial"));
		if (Material.Succeeded()) {
			ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
		}
		//TO-DO: remove magic number here
		ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
		ProjectileMeshComponent->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
		ProjectileMeshComponent->SetupAttachment(RootComponent);
	}
	// Delete the projectile after 3 seconds.
	//InitialLifeSpan = 3.0f;
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
	ProjectileMovementComponent->Velocity = ShootDirection * 1000.0f;
}

// Function that is called when the projectile hits something.
void ASKProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Deactivate();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("HIT!!!"));
	//if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	//{
		//OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	//}
	//Destroy();
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
	GetWorldTimerManager().ClearAllTimersForObject(this);
	OnPooledProjectileDespawn.Broadcast(this);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SKCharacterBase.h"
#include "../Weapons/SKProjectilePool.h"
#include "../Weapons/SKProjectileBase.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASKCharacterBase::ASKCharacterBase()
{
	// Set up bullet pool
	MyProjectilePool = CreateDefaultSubobject<USKProjectilePool>(TEXT("ProjectilePool"));
}

// Called when the game starts or when spawned
void ASKCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASKCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASKCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASKCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

#pragma region Actions
void ASKCharacterBase::Fire() {
	if (MyProjectilePool != nullptr) {
		//Get camera transform
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		//Set muzzleoffset to spawn projectiles slightly in front of the camera
		//TO-DO: remove magic number here
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		//Transform muzzle offset from camera to world space
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		MuzzleLocation.Z = 80.0f;

		//Spawn the projectile at the muzzle
		ASKProjectileBase* Projectile = MyProjectilePool->SpawnPooledProjectile();
		if (Projectile != nullptr) {
			check(GEngine != nullptr);
			Projectile->SetActorLocation(MuzzleLocation);
			Projectile->SetInstigator(this);
			Projectile->FireInDirection(CameraRotation.Vector());
		}
	}
}
#pragma endregion Actions

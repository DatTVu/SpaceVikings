// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "ProjectileBase.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

#pragma region Movements
void ACharacterBase::MoveForward(float Value) {
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(direction, Value);
}

void ACharacterBase::MoveRight(float Value) {
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(direction, Value);
}

void ACharacterBase::StartJump()
{
	bPressedJump = true;
}

void ACharacterBase::StopJump()
{
	bPressedJump = false;
}
#pragma endregion Movements

#pragma region Actions
void ACharacterBase::Fire() {
	if (ProjectileClass) {
		//Get camera transform
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		//Set muzzleoffset to spawn projectiles slightly in front of the camera
		//TO-DO: remove magic number here
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		//Transform muzzle offset from camera to world space
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		//Skew the aim the be slightly upwards
		FRotator MuzzleRotation = CameraRotation;
		//TO-DO: remove magic number here
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			//Spawn the projectile at the muzzle
			AProjectileBase* Projectile = World->SpawnActor<AProjectileBase>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile) {
				//Set the projectile's initial trajectory
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}
#pragma endregion Actions

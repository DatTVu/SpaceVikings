// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 
#include "SpaceVikingsCharacterBase.h"
#include "SpaceVikingsProjectileBase.h"

// Sets default values
ASpaceVikingsCharacterBase::ASpaceVikingsCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Initialize the Camera Boom    
	m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//Setup Camera Boom attachment to the Root component of the class
	m_CameraBoom->SetupAttachment(RootComponent);
	//Set the boolean to use the PawnControlRotation to true.
	m_CameraBoom->bUsePawnControlRotation = true;
	//Initialize the FollowCamera
	m_FollowTopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//Set FollowCamera attachment to the Camera Boom
	m_FollowTopDownCamera->SetupAttachment(m_CameraBoom);
}

// Called when the game starts or when spawned
void ASpaceVikingsCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using Space Viking Character."));
	
}

// Called every frame
void ASpaceVikingsCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpaceVikingsCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up movements
	PlayerInputComponent->BindAxis("MoveForward", this, &ASpaceVikingsCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpaceVikingsCharacterBase::MoveRight);
	// Set up "look" bindings.
	//PlayerInputComponent->BindAxis("Turn", this, &ASpaceVikingsCharacterBase::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("LookUp", this, &ASpaceVikingsCharacterBase::AddControllerPitchInput);
	// Set up "jump" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASpaceVikingsCharacterBase::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASpaceVikingsCharacterBase::StopJump);
	// Set up "fire" bindings
	PlayerInputComponent->BindAction("Fire", IE_Released, this, & ASpaceVikingsCharacterBase::Fire);
}

#pragma region Movements
void ASpaceVikingsCharacterBase::MoveForward(float Value) {
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(direction, Value);
}

void ASpaceVikingsCharacterBase::MoveRight(float Value) {
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(direction, Value);
}

void ASpaceVikingsCharacterBase::StartJump()
{
	bPressedJump = true;
}

void ASpaceVikingsCharacterBase::StopJump()
{
	bPressedJump = false;
}
#pragma endregion Movements

#pragma region Actions
void ASpaceVikingsCharacterBase::Fire() {
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
			ASpaceVikingsProjectileBase* Projectile = World->SpawnActor<ASpaceVikingsProjectileBase>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile) {
				//Set the projectile's initial trajectory
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}
#pragma endregion Actions

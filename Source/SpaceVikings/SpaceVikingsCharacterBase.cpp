// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceVikingsCharacterBase.h"

// Sets default values
ASpaceVikingsCharacterBase::ASpaceVikingsCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	PlayerInputComponent->BindAxis("Turn", this, &ASpaceVikingsCharacterBase::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ASpaceVikingsCharacterBase::AddControllerPitchInput);
	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASpaceVikingsCharacterBase::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASpaceVikingsCharacterBase::StopJump);
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

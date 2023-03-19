// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 
#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
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
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);
	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Spawning Player Character."));
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up movements
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);
	// Set up "look" bindings.
	//PlayerInputComponent->BindAxis("Turn", this, &ASpaceVikingsCharacterBase::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("LookUp", this, &ASpaceVikingsCharacterBase::AddControllerPitchInput);
	// Set up "jump" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterBase::StopJump);
	// Set up "fire" bindings
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACharacterBase::Fire);
}


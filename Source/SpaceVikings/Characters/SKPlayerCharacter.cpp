// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

static const FString PlayerMeshName = "/Game/Assets/LPSD2_Meshes/Pirate/SM_PirateShip_1.SM_PirateShip_1";
// Sets default values
ASKPlayerCharacter::ASKPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//Initialize the Camera Boom    
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//Setup Camera Boom attachment to the Root component of the class
	CameraBoom->SetupAttachment(RootComponent);
	//Set the boolean to use the PawnControlRotation to true.
	CameraBoom->bUsePawnControlRotation = true;
	//Initialize the FollowCamera
	FollowTopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//Set FollowCamera attachment to the Camera Boom
	FollowTopDownCamera->SetupAttachment(CameraBoom);
	//Init vector to hold value for orb combination
	VecOrbCombination.Init(EOrb::NONE, MaxOrbCnt);
	//Set up static mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	UStaticMesh* MyMesh = LoadObject<UStaticMesh>(nullptr, *PlayerMeshName);
	StaticMeshComponent->SetStaticMesh(MyMesh);
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

// Called when the game starts or when spawned
void ASKPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);
	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Spawning Player Character."));
}

// Called every frame
void ASKPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASKPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up movements
	PlayerInputComponent->BindAxis("MoveForward", this, &ASKCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASKCharacterBase::MoveRight);
	// Set up "jump" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASKCharacterBase::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASKCharacterBase::StopJump);
	// Set up "fire" bindings
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASKCharacterBase::Fire);
	// Set up invoke orb bindings
	PlayerInputComponent->BindAction("Quas", IE_Released, this, &ASKPlayerCharacter::HandleQuasPressed);
	PlayerInputComponent->BindAction("Wex", IE_Released, this, &ASKPlayerCharacter::HandleWexPressed);
	PlayerInputComponent->BindAction("Exort", IE_Released, this, &ASKPlayerCharacter::HandleExortPressed);
	PlayerInputComponent->BindAction("Invoke", IE_Released, this, &ASKPlayerCharacter::HandleInvorkPressed);
}


void ASKPlayerCharacter::HandleQuasPressed() {
	HanddleOrb(EOrb::QUAS);
}

void ASKPlayerCharacter::HandleWexPressed() {
	HanddleOrb(EOrb::WEX);
}

void ASKPlayerCharacter::HandleExortPressed() {
	HanddleOrb(EOrb::EXORT);
}

void ASKPlayerCharacter::HanddleOrb(const EOrb eVal) {
	VecOrbCombination[CurrentOrbSlot] = eVal;
	CurrentOrbSlot = (CurrentOrbSlot + 1) % MaxOrbCnt;
}

void ASKPlayerCharacter::HandleInvorkPressed() {

	int invokeSum = 0;

	for (auto num : VecOrbCombination) {
		invokeSum += num;
	}
#ifdef UE_BUILD_DEBUG
	FString skillInvoked = "";
	switch (invokeSum) {
	case EInvoke::QQ:
		skillInvoked = "QQ";
		break;
	case EInvoke::QW:
		skillInvoked = "QW / WQ";
		break;
	case EInvoke::QE:
		skillInvoked = "QE / EQ";
		break;
	case EInvoke::WW:
		skillInvoked = "WW";
		break;
	case EInvoke::WE:
		skillInvoked = "WE / EW";
		break;
	case EInvoke::EE:
		skillInvoked = "EE";
		break;
	case EInvoke::NN:
	case EInvoke::QN:
	case EInvoke::WN:
	case EInvoke::EN:
	default:
		skillInvoked = "Invalid";
		break;
	}
	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, skillInvoked);
#else
#endif
}

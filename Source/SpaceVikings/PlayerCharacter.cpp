// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 


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
	//Init vector to hold value for orb combination
	m_vecOrbCombination.Init(EOrb::NONE, m_maxOrbCnt);
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
	// Set up "jump" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterBase::StopJump);
	// Set up "fire" bindings
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACharacterBase::Fire);
	// Set up invoke orb bindings
	PlayerInputComponent->BindAction("Quas", IE_Released, this, &APlayerCharacter::HandleQuasPressed);
	PlayerInputComponent->BindAction("Wex", IE_Released, this, &APlayerCharacter::HandleWexPressed);
	PlayerInputComponent->BindAction("Exort", IE_Released, this, &APlayerCharacter::HandleExortPressed);
	PlayerInputComponent->BindAction("Invoke", IE_Released, this, &APlayerCharacter::HandleInvorkPressed);
}


void APlayerCharacter::HandleQuasPressed() {
	HanddleOrb(EOrb::QUAS);
}

void APlayerCharacter::HandleWexPressed() {
	HanddleOrb(EOrb::WEX);
}

void APlayerCharacter::HandleExortPressed() {
	HanddleOrb(EOrb::EXORT);
}

void APlayerCharacter::HanddleOrb(const EOrb eVal) {
	m_vecOrbCombination[m_currentOrbSlot] = eVal;
	m_currentOrbSlot = (m_currentOrbSlot + 1) % m_maxOrbCnt;
}

void APlayerCharacter::HandleInvorkPressed() {
	
	int invokeSum = 0;

	for (auto num : m_vecOrbCombination) {
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

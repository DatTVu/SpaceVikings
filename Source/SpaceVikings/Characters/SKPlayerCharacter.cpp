// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../Abilities/SKAbilitySystemComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Engine/StaticMesh.h"

static const FString PlayerMeshName = "/Game/Assets/LPSD2_Meshes/Pirate/SM_PirateShip_1.SM_PirateShip_1";
// Sets default values
ASKPlayerCharacter::ASKPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Init vector to hold value for orb combination
	VecOrbCombination.Init(EOrb::NONE, MaxOrbCnt);
	//Set up static mesh
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollision;
	BoxCollision->SetCollisionProfileName(TEXT("Pawn"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Assets/LPSD2_Meshes/Pirate/SM_PirateShip_1.SM_PirateShip_1"));
	if (SphereVisualAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(SphereVisualAsset.Object);
	}
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ASKPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Spawning Player Character."));
	if (APlayerController* PlayerCharacterController = Cast<APlayerController>(this->GetController()))
	{
		//Array to contain found Camera Actors
		TArray<AActor*> FoundActors;
		//Utility function to populate array with all Camera Actors in the level
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CameraToFind, FoundActors);
		//Sets Player Controller view to the first CameraActor found 
		PlayerCharacterController->SetViewTargetWithBlend(FoundActors[0], 1.0, EViewTargetBlendFunction::VTBlend_Linear);
	}
}

// Called every frame
void ASKPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float speed = 300.0f;
	if (HorizontalMovement != 0.0) 
	{
		FVector newLocation = GetRootComponent()->GetComponentLocation();
		newLocation.Y += DeltaTime * speed * HorizontalMovement;
		newLocation.Y = FMath::Min(FMath::Max(newLocation.Y, -600.0f), 600.0f);
		SetActorLocation(newLocation);
	}

	if (VerticalMovement != 0.0) {
		FVector newLocation = GetRootComponent()->GetComponentLocation();
		newLocation.X += DeltaTime * speed * VerticalMovement;
		newLocation.X = FMath::Min(FMath::Max(newLocation.X, -600.0f), 600.0f);
		SetActorLocation(newLocation);
	}
}

// Called to bind functionality to input
void ASKPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up movements
	PlayerInputComponent->BindAxis("MoveForward", this, &ASKPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASKPlayerCharacter::MoveRight);
	// Set up "fire" bindings
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASKCharacterBase::Fire);
	// Set up invoke orb bindings
	PlayerInputComponent->BindAction("Quas", IE_Released, this, &ASKPlayerCharacter::HandleQuasPressed);
	PlayerInputComponent->BindAction("Wex", IE_Released, this, &ASKPlayerCharacter::HandleWexPressed);
	PlayerInputComponent->BindAction("Exort", IE_Released, this, &ASKPlayerCharacter::HandleExortPressed);
	PlayerInputComponent->BindAction("Invoke", IE_Released, this, &ASKPlayerCharacter::HandleInvorkPressed);
}

void ASKPlayerCharacter::MoveForward(float Value)
{
	VerticalMovement = Value;
}

void ASKPlayerCharacter::MoveRight(float Value)
{
	HorizontalMovement = Value;
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

void ASKPlayerCharacter::OnBeginOverlap(AActor* otherActor)
{
}

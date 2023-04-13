// Fill out your copyright notice in the Description page of Project Settings.


#include "SKEnemyCharacter.h"
//const FString PlayerMeshName = "/Game/Assets/LPSD2_Meshes/Pirate/SM_PirateShip_1.SM_PirateShip_1";
static const FString EnemyMeshName = "/Game/Assets/LPSD2_Meshes/Beach/SM_SmallBoat_1.SM_SmallBoat_1";
ASKEnemyCharacter::ASKEnemyCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//Set up static mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	UStaticMesh* MyMesh = LoadObject<UStaticMesh>(nullptr, *EnemyMeshName);
	StaticMeshComponent->SetStaticMesh(MyMesh);
	StaticMeshComponent->SetupAttachment(RootComponent);
	//StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

bool ASKEnemyCharacter::IsActive()
{
	return BIsActive;
}

void ASKEnemyCharacter::SetActive(bool isActive)
{
	BIsActive = isActive;
	SetActorHiddenInGame(!isActive);
	//GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &ASKEnemyCharacter::Deactivate, LifeSpan, false);
}

void ASKEnemyCharacter::SetLifeSpan(float lifeSpan)
{
	LifeSpan = lifeSpan;
}

int ASKEnemyCharacter::GetPoolIndex()
{
	return PoolIndex;
}

void ASKEnemyCharacter::SetPoolIndex(int idx)
{
	PoolIndex = idx;
}

void ASKEnemyCharacter::Tick(float DeltaTime)
{
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(direction, 0.1);
}


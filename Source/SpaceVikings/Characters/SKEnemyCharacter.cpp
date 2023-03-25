// Fill out your copyright notice in the Description page of Project Settings.


#include "SKEnemyCharacter.h"
static const FString EnemyMeshName = "/Game/Assets/LPSD2_Meshes/Flora/SM_BananaTree_1.SM_BananaTree_1";
ASKEnemyCharacter::ASKEnemyCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//Set up static mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	UStaticMesh* MyMesh = LoadObject<UStaticMesh>(nullptr, *EnemyMeshName);
	StaticMeshComponent->SetStaticMesh(MyMesh);
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
}


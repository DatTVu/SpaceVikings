// Fill out your copyright notice in the Description page of Project Settings.


#include "SKTreasure.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASKTreasure::ASKTreasure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//Set up static mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollision;
	StaticMeshComponent->SetupAttachment(BoxCollision);
}

void ASKTreasure::DecreaseHP(float val)
{
	m_HP -= val;
	if (m_HP < 0) {
		m_HP = 0.f;
	}
}

// Called when the game starts or when spawned
void ASKTreasure::BeginPlay()
{
	Super::BeginPlay();
}

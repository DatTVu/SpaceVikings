#include "SKEnemyCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

static const FString EnemyMeshName = "/Game/Assets/LPSD2_Meshes/Beach/SM_SmallBoat_1.SM_SmallBoat_1";

ASKEnemyCharacter::ASKEnemyCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//Set up static mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollision;
	StaticMeshComponent->SetupAttachment(BoxCollision);
}

void ASKEnemyCharacter::Deactivate()
{
	SetActive(false);
	SetCanMove(false);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("HIT ENEMIES!!!"));
	OnPooledEnemyDespawn.Broadcast(this);
}

bool ASKEnemyCharacter::IsActive()
{
	return m_IsActive;
}

void ASKEnemyCharacter::SetActive(bool isActive)
{
	m_IsActive = isActive;
}

void ASKEnemyCharacter::SetCanMove(bool canMove)
{
	m_CanMove = canMove;
}

bool ASKEnemyCharacter::CanMove()
{
	return m_CanMove;
}

int ASKEnemyCharacter::GetRowIndex()
{
	return m_rowIndex;
}

int ASKEnemyCharacter::GetColIndex()
{
	return m_colIndex;
}

void ASKEnemyCharacter::SetRowIndex(int idx)
{
	m_rowIndex = idx;
}

void ASKEnemyCharacter::SetColIndex(int idx)
{
	m_colIndex = idx;
}

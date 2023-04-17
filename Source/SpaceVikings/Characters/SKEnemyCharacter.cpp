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
	ShootDirection = FVector(1.0f, 0.0f, 0.0f);
}

void ASKEnemyCharacter::Deactivate()
{
	SetActive(false);
}

bool ASKEnemyCharacter::IsActive()
{
	return m_IsActive;
}

void ASKEnemyCharacter::SetActive(bool isActive)
{
	m_IsActive = isActive;
}

int ASKEnemyCharacter::GetPoolIndex()
{
	return m_PoolIndex;
}

void ASKEnemyCharacter::SetPoolIndex(int idx)
{
	m_PoolIndex = idx;
}

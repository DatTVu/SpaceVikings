// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../SpaceVikings.h"
#include "GameFramework/Pawn.h"
#include "SKCharacterBase.generated.h"

class USKProjectilePool;
class ASKProjectileBase;

UCLASS()
class SPACEVIKINGS_API ASKCharacterBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASKCharacterBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called when this Pawn is possessed. Only called on the server (or in standalone).
	virtual void PossessedBy(AController* NewController) override;
	// Fire function
	UFUNCTION()
	void Fire();
	
public:
	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;
protected:
	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	class USKProjectilePool* MyProjectilePool;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

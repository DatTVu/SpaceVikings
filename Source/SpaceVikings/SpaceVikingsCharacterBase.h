// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceVikingsCharacterBase.generated.h"

UCLASS()
class SPACEVIKINGS_API ASpaceVikingsCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpaceVikingsCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called to handle movements
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	// Sets jump flag when key is pressed.
	UFUNCTION()
		void StartJump();
	// Clears jump flag when key is released.
	UFUNCTION()
		void StopJump();

};

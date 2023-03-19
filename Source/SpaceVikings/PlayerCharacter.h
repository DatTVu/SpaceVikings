// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SPACEVIKINGS_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// TopDown camera.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* m_CameraBoom;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* m_FollowTopDownCamera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

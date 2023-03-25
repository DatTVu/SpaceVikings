// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../SpaceVikings.h"
#include "SKCharacterBase.h"
#include "SKPlayerCharacter.generated.h"

UCLASS()
class SPACEVIKINGS_API ASKPlayerCharacter : public ASKCharacterBase
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ASKPlayerCharacter();

private:
	//TO-DO: refactore this enum. now it needs to declare infront of TArray
	enum EOrb {
		NONE = 0,
		QUAS = 1,
		WEX = 7,
		EXORT = 11
	};
	enum EInvoke {
		NN = 0,
		QN = 1,
		WN = 7,
		EN = 11,
		QQ = 2,
		QW = 8,
		QE = 12,
		WW = 14,
		WE = 18,
		EE = 22
	};
	TArray<EOrb> VecOrbCombination;
	int CurrentOrbSlot = 0;
	const int MaxOrbCnt = 2;
	FORCEINLINE void HanddleOrb(const EOrb eVal);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// TopDown camera.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
		class USpringArmComponent* CameraBoom;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* FollowTopDownCamera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaticMesh")
		class UStaticMeshComponent* StaticMeshComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void HandleQuasPressed();
	UFUNCTION()
		void HandleWexPressed();
	UFUNCTION()
		void HandleExortPressed();
	UFUNCTION()
		void HandleInvorkPressed();
};
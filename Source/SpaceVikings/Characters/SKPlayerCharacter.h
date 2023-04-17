// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../SpaceVikings.h"
#include "SKCharacterBase.h"
#include "SKPlayerCharacter.generated.h"

class ACameraActor;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACameraActor> CameraToFind;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	//virtual void PossessedBy(AController* NewController) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called to handle movements
	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaticMesh")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoxCollision")
	class UBoxComponent* BoxCollision;

	float HorizontalMovement = 0.0f;
	float VerticalMovement = 0.0f;
	

	UFUNCTION()
		void HandleQuasPressed();
	UFUNCTION()
		void HandleWexPressed();
	UFUNCTION()
		void HandleExortPressed();
	UFUNCTION()
		void HandleInvorkPressed();
	UFUNCTION()
		void OnBeginOverlap(AActor* otherActor);
};
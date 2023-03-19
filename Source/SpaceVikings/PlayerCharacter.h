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
	TArray<EOrb> m_vecOrbCombination;
	int m_currentOrbSlot = 0;
	const int m_maxOrbCnt = 2;
	FORCEINLINE void HanddleOrb(const EOrb eVal);
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

	UFUNCTION()
	void HandleQuasPressed();
	UFUNCTION()
	void HandleWexPressed();
	UFUNCTION()
	void HandleExortPressed();
	UFUNCTION()
	void HandleInvorkPressed();
};

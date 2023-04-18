// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SKTreasure.generated.h"

UCLASS()
class SPACEVIKINGS_API ASKTreasure : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASKTreasure();
	float GetMaxHP() { return m_MaxHP; }
	float GetHP() { return m_HP; }
	void  DecreaseHP(float val);
	void  Deactivate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaticMesh")
		class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoxCollision")
		class UBoxComponent* BoxCollision;
private:
	float m_MaxHP = 5.0f;
	float m_HP = 5.0f;
};

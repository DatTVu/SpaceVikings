// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Characters/SKCharacterBase.h"
#include "SKTreasure.generated.h"

UCLASS()
class SPACEVIKINGS_API ASKTreasure : public ASKCharacterBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASKTreasure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaticMesh")
		class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoxCollision")
		class UBoxComponent* BoxCollision;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../SpaceVikings.h"
#include "../Abilities/SKAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "Components/StaticMeshComponent.h"
#include "SKCharacterBase.generated.h"

class USKAttributeSetBase;
class USKGameplayAbility;
class USKProjectilePool;
class ASKProjectileBase;
class UGameplayEffect;
class UStaticMeshComponent;

UCLASS()
class SPACEVIKINGS_API ASKCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASKCharacterBase();

	/* Returns the ability system component to use for this actor.
	It may live on another actor, such as a Pawn using the PlayerState's component */
	UAbilitySystemComponent* GetAbilitySystemComponent() const override {
	return SKAbilitySystemComponent;
	};

	/** Returns current health, will be 0 if dead */
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	/** Returns current mana */
	UFUNCTION(BlueprintCallable)
	virtual float GetMana() const;

	/** Returns maximum mana, mana will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxMana() const;

	/** Returns current movement speed */
	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Called when this Pawn is possessed. Only called on the server (or in standalone).
	 * @param NewController The controller possessing this pawn
	 */
	virtual void PossessedBy(AController* NewController) override;

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

	// Fire function
	UFUNCTION()
		void Fire();
public:
	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StaticMesh")
	UStaticMeshComponent* StaticMeshComponent;
	/** List of attributes modified by the ability system */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USKAttributeSetBase* AttributeSet;
	/** The component used to handle ability system interactions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USKAbilitySystemComponent* SKAbilitySystemComponent;
	/* Array to hold initial gameplay abilities*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability")
	TArray<TSubclassOf<USKGameplayAbility>> InitialGameplayAbilities;

	/* Array to hold initial gameplay effects*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameplayEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability")
	TSubclassOf<UGameplayEffect> ExhaustionEffectClass;

	/* Keep track if abilites granted only once during initialization*/
	bool bIsCharacterAbilitiesGranted = false;

	/* Keep track if effects granted only once during initialization*/
	bool bIsCharacterEffectsGranted = false;

	/* Keep track if abilities are bound to input*/
	bool bIsInputBound = false;

	void AddInitialCharacterAbilities();

	void AddInitialCharacterEffects();

	void SetupAbilitiesInputs();

	void OnManaChanged(const FOnAttributeChangeData& Data);

	//Apply exhaustion effect after character is out of stamina
	void ApplyExhaustionEffect();

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	class USKProjectilePool* MyProjectilePool;
	//UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	//TSubclassOf<ASKProjectileBase> ProjectileClass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	friend USKAttributeSetBase;
};

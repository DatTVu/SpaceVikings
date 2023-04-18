// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../SpaceVikings.h"
#include "SKCharacterBase.h"
#include "../Abilities/SKAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "SKPlayerCharacter.generated.h"

class ACameraActor;
class USKAttributeSetBase;
class USKGameplayAbility;
class USKProjectilePool;
class ASKProjectileBase;
class UGameplayEffect;

UCLASS()
class SPACEVIKINGS_API ASKPlayerCharacter : public ASKCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ASKPlayerCharacter();

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

	UFUNCTION(BlueprintCallable)
		float GetPlayerHealth() { return m_playerHealth; }

	UFUNCTION(BlueprintCallable)
		float GetPlayerMaxHealth() { return m_playerMaxHealth; }

	UFUNCTION(BlueprintCallable)
		void DecreasePlayerHealth(float val);

	UFUNCTION(BlueprintCallable)
		int GetPlayerScore() { return m_playerScore; }

	UFUNCTION(BlueprintCallable)
		int GetPlayerHighScore() { return m_playerHighScore; }

	UFUNCTION(BlueprintCallable)
		void IncreasePlayerScore(int val) { m_playerScore += val; }

	bool IsInvulnerable() { return m_IsInvulnerable; }
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
	float m_HorizontalMovement = 0.0f;
	float m_VerticalMovement = 0.0f;
	bool m_IsInvulnerable = false;
	float m_InvulnerableTimer = 0.0f;
	float m_playerMaxHealth = 5.0f;
	float m_playerHealth = 5.0f;
	int m_playerScore = 0;
	int m_playerHighScore = 0;
	FORCEINLINE void HanddleOrb(const EOrb eVal);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACameraActor> CameraToFind;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
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
	friend USKAttributeSetBase;
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

	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION()
		void HandleQuasPressed();
	UFUNCTION()
		void HandleWexPressed();
	UFUNCTION()
		void HandleExortPressed();
	UFUNCTION()
		void HandleInvorkPressed();
};
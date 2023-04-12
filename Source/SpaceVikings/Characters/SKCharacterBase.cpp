// Fill out your copyright notice in the Description page of Project Settings.


#include "SKCharacterBase.h"
#include "../Abilities/SKAttributeSetBase.h"
#include "../Abilities/SKGameplayAbility.h"
#include "../Abilities/SKAbilitySystemComponent.h"
#include "../SKPlayerController.h"
#include "../Weapons/SKProjectilePool.h"
#include "../Weapons/SKProjectileBase.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASKCharacterBase::ASKCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Create ability system component, and set it to be explicitly replicated
	SKAbilitySystemComponent = CreateDefaultSubobject<USKAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	SKAbilitySystemComponent->SetIsReplicated(true);
	SKAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<USKAttributeSetBase>(TEXT("AttributeSet"));
	MyProjectilePool = CreateDefaultSubobject<USKProjectilePool>(TEXT("ProjectilePool"));
	bIsCharacterAbilitiesGranted = false;
	bIsCharacterEffectsGranted = false;
	bIsInputBound = false;
}

// Called when the game starts or when spawned
void ASKCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}


void ASKCharacterBase::OnManaChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0) {
		ApplyExhaustionEffect();
	}

	ASKPlayerController* CharacterController = Cast<ASKPlayerController>(GetController());
	if (IsValid(CharacterController) && IsValid(AttributeSet)) {
		CharacterController->BP_OnManaChanged(Data.NewValue, AttributeSet->GetMaxMana());
	}
}

void ASKCharacterBase::ApplyExhaustionEffect()
{
	if (!SKAbilitySystemComponent || !IsValid(ExhaustionEffectClass)) {
		return;
	}

	ASKPlayerController* CharacterController = Cast<ASKPlayerController>(GetController());
	if (IsValid(CharacterController)) {
		CharacterController->BP_OnExhaustion();
	}

	FGameplayEffectContextHandle EffectContextHandle = SKAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle CurrentGameplayEffectSpecHandle = SKAbilitySystemComponent->MakeOutgoingSpec(ExhaustionEffectClass, 1.0f, EffectContextHandle);
	if (CurrentGameplayEffectSpecHandle.IsValid()) {
		SKAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*CurrentGameplayEffectSpecHandle.Data.Get(), SKAbilitySystemComponent);
	}

}

// Called every frame
void ASKCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASKCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupAbilitiesInputs();
}

void ASKCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (IsValid(SKAbilitySystemComponent) && AttributeSet) {
		SKAbilitySystemComponent->InitAbilityActorInfo(this, this);
		//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &ASKCharacterBase::OnManaChanged);
		AddInitialCharacterAbilities();
		AddInitialCharacterEffects();
	}
}

void ASKCharacterBase::AddInitialCharacterAbilities() {
	if (!SKAbilitySystemComponent || bIsCharacterAbilitiesGranted) {
		return;
	}

	// Grant abilities:
	for (TSubclassOf<USKGameplayAbility> CurrentGameplayAbilityClass : InitialGameplayAbilities)
	{
		if (IsValid(CurrentGameplayAbilityClass)) {
			USKGameplayAbility* CurrentAbility = CurrentGameplayAbilityClass->GetDefaultObject<USKGameplayAbility>();
			if (IsValid(CurrentAbility)) {
				FGameplayAbilitySpec AbilitySpec(CurrentAbility, 1, static_cast<int32>(CurrentAbility->AbilityInputID), this);
				SKAbilitySystemComponent->GiveAbility(AbilitySpec);
			}
		}
	}
	bIsCharacterAbilitiesGranted = true;
}

void ASKCharacterBase::AddInitialCharacterEffects() {
	if (!SKAbilitySystemComponent || bIsCharacterEffectsGranted) {
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = SKAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> CurrentGameplayEffectClass : InitialGameplayEffects) {
		if (IsValid(CurrentGameplayEffectClass)) {
			FGameplayEffectSpecHandle CurrentGameplayEffectSpecHandle = SKAbilitySystemComponent->MakeOutgoingSpec(CurrentGameplayEffectClass, 1.0f, EffectContextHandle);
			if (CurrentGameplayEffectSpecHandle.IsValid()) {
				SKAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*CurrentGameplayEffectSpecHandle.Data.Get(), SKAbilitySystemComponent);
			}
		}
	}
	bIsCharacterAbilitiesGranted = true;
}

void ASKCharacterBase::SetupAbilitiesInputs() {
	if (!SKAbilitySystemComponent || !InputComponent || bIsInputBound) {
		return;
	}

	SKAbilitySystemComponent->BindAbilityActivationToInputComponent(
		InputComponent,
		FGameplayAbilityInputBinds(
			"Confirm",
			"Cancel",
			"ESK_AbilityInput",
			static_cast<uint32>(ESK_AbilityInput::Confirm),
			static_cast<uint32>(ESK_AbilityInput::Cancel)
		)
	);
	bIsInputBound = true;
}

#pragma region Movements

void ASKCharacterBase::MoveForward(float Value) {
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(direction, Value);
}

void ASKCharacterBase::MoveRight(float Value) {
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(direction, Value);
}

void ASKCharacterBase::StartJump()
{
	bPressedJump = true;
}

void ASKCharacterBase::StopJump()
{
	bPressedJump = false;
}
#pragma endregion Movements

#pragma region Actions
void ASKCharacterBase::Fire() {
	if (MyProjectilePool != nullptr) {
		//Get camera transform
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		//Set muzzleoffset to spawn projectiles slightly in front of the camera
		//TO-DO: remove magic number here
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		//Transform muzzle offset from camera to world space
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		MuzzleLocation.Z = 0.0f;

		//Skew the aim the be slightly upwards
		//FRotator MuzzleRotation = CameraRotation;
		//TO-DO: remove magic number here
		//MuzzleRotation.Pitch += 10.0f;


		//Spawn the projectile at the muzzle
		ASKProjectileBase* Projectile = MyProjectilePool->SpawnPooledProjectile();
		if (Projectile != nullptr) {
			check(GEngine != nullptr);
			Projectile->SetActorLocation(MuzzleLocation);
			Projectile->SetInstigator(GetInstigator());
			Projectile->FireInDirection(CameraRotation.Vector());
		}
	}
}
#pragma endregion Actions

#pragma region AttributeSet
float ASKCharacterBase::GetHealth() const
{
	if (!AttributeSet)
		return 1.f;

	return AttributeSet->GetHealth();
}

float ASKCharacterBase::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float ASKCharacterBase::GetMana() const
{
	return AttributeSet->GetMana();
}

float ASKCharacterBase::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float ASKCharacterBase::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}
#pragma endregion AttributSet
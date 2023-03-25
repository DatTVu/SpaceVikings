// Fill out your copyright notice in the Description page of Project Settings.


#include "SKCharacterBase.h"

#include "../Weapons/SKProjectileBase.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "../Abilities/SKAttributeSetBase.h"
#include "../Abilities/SKGameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <SpaceVikings/SKPlayerController.h>

// Sets default values
ASKCharacterBase::ASKCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<USKAttributeSetBase>(TEXT("AttributeSet"));
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
	if (!IsValid(AbilitySystemComponent) || !IsValid(ExhaustionEffectClass)) {
		return;
	}

	ASKPlayerController* CharacterController = Cast<ASKPlayerController>(GetController());
	if (IsValid(CharacterController)) {
		CharacterController->BP_OnExhaustion();
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle CurrentGameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ExhaustionEffectClass, 1.0f, EffectContextHandle);
	if (CurrentGameplayEffectSpecHandle.IsValid()) {
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*CurrentGameplayEffectSpecHandle.Data.Get(), AbilitySystemComponent);
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
	if (AbilitySystemComponent && AttributeSet) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &ASKCharacterBase::OnManaChanged);
		AddInitialCharacterAbilities();
		AddInitialCharacterEffects();
	}
}

void ASKCharacterBase::AddInitialCharacterAbilities() {
	if (!AbilitySystemComponent || bIsCharacterAbilitiesGranted) {
		return;
	}

	for (TSubclassOf<USKGameplayAbility> CurrentGameplayAbilityClass : InitialGameplayAbilities) {
		if (IsValid(CurrentGameplayAbilityClass)) {
			USKGameplayAbility* CurrentAbility = CurrentGameplayAbilityClass->GetDefaultObject<USKGameplayAbility>();
			if (IsValid(CurrentAbility)) {
				FGameplayAbilitySpec AbilitySpec(CurrentAbility, 1, static_cast<int32>(CurrentAbility->AbilityInputID), this);
				AbilitySystemComponent->GiveAbility(AbilitySpec);
			}
	}
	}
	bIsCharacterAbilitiesGranted = true;
}

void ASKCharacterBase::AddInitialCharacterEffects() {
	if (!AbilitySystemComponent || bIsCharacterEffectsGranted) {
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> CurrentGameplayEffectClass : InitialGameplayEffects) {
		if (IsValid(CurrentGameplayEffectClass)) {
			FGameplayEffectSpecHandle CurrentGameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CurrentGameplayEffectClass, 1.0f, EffectContextHandle);
			if (CurrentGameplayEffectSpecHandle.IsValid()) {
				//FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*CurrentGameplayEffectSpecHandle.Data.Get(), AbilitySystemComponent);
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*CurrentGameplayEffectSpecHandle.Data.Get(), AbilitySystemComponent);
			}
		}
	}
	bIsCharacterAbilitiesGranted = true;
}

void ASKCharacterBase::SetupAbilitiesInputs() {
	if (!AbilitySystemComponent || !InputComponent || bIsInputBound) {
		return;
	}

	AbilitySystemComponent->BindAbilityActivationToInputComponent(
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
	if (ProjectileClass) {
		//Get camera transform
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		//Set muzzleoffset to spawn projectiles slightly in front of the camera
		//TO-DO: remove magic number here
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		//Transform muzzle offset from camera to world space
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		//Skew the aim the be slightly upwards
		FRotator MuzzleRotation = CameraRotation;
		//TO-DO: remove magic number here
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			//Spawn the projectile at the muzzle
			ASKProjectileBase* Projectile = World->SpawnActor<ASKProjectileBase>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile) {
				//Set the projectile's initial trajectory
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
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
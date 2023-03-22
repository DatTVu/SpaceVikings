// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "ProjectileBase.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "AttributeSetBase.h"
#include "SKGameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <SpaceVikings/SKPlayerController.h>

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSet"));
	bIsCharacterAbilitiesGranted = false;
	bIsCharacterEffectsGranted = false;
	bIsInputBound = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}


void ACharacterBase::OnManaChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0){
		ApplyExhaustionEffect();
	}

	ASKPlayerController* CharacterController = Cast<ASKPlayerController>(GetController());
	if (IsValid(CharacterController) && IsValid(AttributeSet)) {
		CharacterController->BP_OnManaChanged(Data.NewValue, AttributeSet->GetMaxMana());
	}
}

void ACharacterBase::ApplyExhaustionEffect()
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
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupAbilitiesInputs();
}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if ( AbilitySystemComponent && AttributeSet) {
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &ACharacterBase::OnManaChanged);
		AddInitialCharacterAbilities();
		AddInitialCharacterEffects();
	}
}

void ACharacterBase::AddInitialCharacterAbilities() {
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

void ACharacterBase::AddInitialCharacterEffects() {
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

void ACharacterBase::SetupAbilitiesInputs() {
	if (!AbilitySystemComponent || !InputComponent|| bIsInputBound) {
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

void ACharacterBase::MoveForward(float Value) {
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(direction, Value);
}

void ACharacterBase::MoveRight(float Value) {
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(direction, Value);
}

void ACharacterBase::StartJump()
{
	bPressedJump = true;
}

void ACharacterBase::StopJump()
{
	bPressedJump = false;
}
#pragma endregion Movements

#pragma region Actions
void ACharacterBase::Fire() {
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
			AProjectileBase* Projectile = World->SpawnActor<AProjectileBase>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
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
float ACharacterBase::GetHealth() const
{
	if (!AttributeSet)
		return 1.f;

	return AttributeSet->GetHealth();
}

float ACharacterBase::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float ACharacterBase::GetMana() const
{
	return AttributeSet->GetMana();
}

float ACharacterBase::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float ACharacterBase::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}
#pragma endregion AttributSet
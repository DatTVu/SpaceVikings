// Fill out your copyright notice in the Description page of Project Settings.


#include "SKPlayerCharacter.h"
#include "Camera/CameraComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Engine/StaticMesh.h"

#include "../Abilities/SKAbilitySystemComponent.h"
#include "../Abilities/SKAttributeSetBase.h"
#include "../Abilities/SKGameplayAbility.h"
#include "../SKPlayerController.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

static const FString PlayerMeshName = "/Game/Assets/LPSD2_Meshes/Pirate/SM_PirateShip_1.SM_PirateShip_1";
// Sets default values
ASKPlayerCharacter::ASKPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.TickGroup = ETickingGroup::TG_DuringPhysics;
	//Init vector to hold value for orb combination
	VecOrbCombination.Init(EOrb::NONE, MaxOrbCnt);
	//Set up static mesh
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollision;
	BoxCollision->SetCollisionProfileName(TEXT("Pawn"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Assets/LPSD2_Meshes/Pirate/SM_PirateShip_1.SM_PirateShip_1"));
	if (SphereVisualAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(SphereVisualAsset.Object);
	}
	// Create ability system component, and set it to be explicitly replicated
	SKAbilitySystemComponent = CreateDefaultSubobject<USKAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	SKAbilitySystemComponent->SetIsReplicated(true);
	SKAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<USKAttributeSetBase>(TEXT("AttributeSet"));
	bIsCharacterAbilitiesGranted = false;
	bIsCharacterEffectsGranted = false;
	bIsInputBound = false;

	//
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

#pragma region AttributeSet
float ASKPlayerCharacter::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float ASKPlayerCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float ASKPlayerCharacter::GetMana() const
{
	return AttributeSet->GetMana();
}

float ASKPlayerCharacter::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float ASKPlayerCharacter::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

void  ASKPlayerCharacter::DecreasePlayerHealth(float val)
{
	m_playerHealth -= val;
	if (m_playerHealth < 0) {
		m_playerHealth = 0.0f;
	}
}
#pragma endregion AttributeSet

// Called when the game starts or when spawned
void ASKPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Spawning Player Character."));
	if (APlayerController* PlayerCharacterController = Cast<APlayerController>(this->GetController()))
	{
		//Array to contain found Camera Actors
		TArray<AActor*> FoundActors;
		//Utility function to populate array with all Camera Actors in the level
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), CameraToFind, FoundActors);
		//Sets Player Controller view to the first CameraActor found 
		PlayerCharacterController->SetViewTargetWithBlend(FoundActors[0], 1.0, EViewTargetBlendFunction::VTBlend_Linear);
	}
	if (IsValid(SKAbilitySystemComponent) && AttributeSet) {
		SKAbilitySystemComponent->InitAbilityActorInfo(this, this);
		SKAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &ASKPlayerCharacter::OnManaChanged);
		AddInitialCharacterAbilities();
		AddInitialCharacterEffects();
	}
}

#pragma region Abilities
void ASKPlayerCharacter::AddInitialCharacterAbilities()
{
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

void ASKPlayerCharacter::AddInitialCharacterEffects()
{
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

void ASKPlayerCharacter::SetupAbilitiesInputs()
{
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
#pragma endregion Abilities

void ASKPlayerCharacter::OnManaChanged(const FOnAttributeChangeData& Data)
{
	ASKPlayerController* CharacterController = Cast<ASKPlayerController>(GetController());
	if (IsValid(CharacterController) && IsValid(AttributeSet)) {
		CharacterController->BP_OnManaChanged(Data.NewValue, AttributeSet->GetMaxMana());
	}
}

// Called every frame
void ASKPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float speed = 300.0f;
	if (m_IsInvulnerable) {
		m_InvulnerableTimer -= DeltaTime;
		if (m_InvulnerableTimer < 0) {
			m_IsInvulnerable = false;
		}
	}
	if (m_HorizontalMovement != 0.0) 
	{
		FVector newLocation = GetRootComponent()->GetComponentLocation();
		newLocation.Y += DeltaTime * speed * m_HorizontalMovement;
		newLocation.Y = FMath::Min(FMath::Max(newLocation.Y, -600.0f), 600.0f);
		SetActorLocation(newLocation);
	}

	if (m_VerticalMovement != 0.0) {
		FVector newLocation = GetRootComponent()->GetComponentLocation();
		newLocation.X += DeltaTime * speed * m_VerticalMovement;
		newLocation.X = FMath::Min(FMath::Max(newLocation.X, -600.0f), 600.0f);
		SetActorLocation(newLocation);
	}
}

// Called to bind functionality to input
void ASKPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up movements
	PlayerInputComponent->BindAxis("MoveForward", this, &ASKPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASKPlayerCharacter::MoveRight);
	// Set up "fire" bindings
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASKCharacterBase::Fire);
	// Set up invoke orb bindings
	PlayerInputComponent->BindAction("Quas", IE_Released, this, &ASKPlayerCharacter::HandleQuasPressed);
	PlayerInputComponent->BindAction("Wex", IE_Released, this, &ASKPlayerCharacter::HandleWexPressed);
	PlayerInputComponent->BindAction("Exort", IE_Released, this, &ASKPlayerCharacter::HandleExortPressed);
	PlayerInputComponent->BindAction("Invoke", IE_Released, this, &ASKPlayerCharacter::HandleInvorkPressed);
	SetupAbilitiesInputs();
}

void ASKPlayerCharacter::MoveForward(float Value)
{
	m_VerticalMovement = Value;
}

void ASKPlayerCharacter::MoveRight(float Value)
{
	m_HorizontalMovement = Value;
}


void ASKPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ASKPlayerCharacter::HandleQuasPressed() {
	HanddleOrb(EOrb::QUAS);
}

void ASKPlayerCharacter::HandleWexPressed() {
	HanddleOrb(EOrb::WEX);
}

void ASKPlayerCharacter::HandleExortPressed() {
	HanddleOrb(EOrb::EXORT);
}

void ASKPlayerCharacter::HanddleOrb(const EOrb eVal) {
	VecOrbCombination[CurrentOrbSlot] = eVal;
	CurrentOrbSlot = (CurrentOrbSlot + 1) % MaxOrbCnt;
}

void ASKPlayerCharacter::HandleInvorkPressed() {
	int invokeSum = 0;
	for (auto num : VecOrbCombination) {
		invokeSum += num;
	}
#ifdef UE_BUILD_DEBUG
	FString skillInvoked = "";
	switch (invokeSum) {
	case EInvoke::QQ:
		skillInvoked = "QQ";
		break;
	case EInvoke::QW:
		skillInvoked = "QW / WQ";
		break;
	case EInvoke::QE:
		skillInvoked = "QE / EQ";
		break;
	case EInvoke::WW:
		skillInvoked = "WW";
		break;
	case EInvoke::WE:
		skillInvoked = "WE / EW";
		break;
	case EInvoke::EE:
		skillInvoked = "EE";
		break;
	case EInvoke::NN:
	case EInvoke::QN:
	case EInvoke::WN:
	case EInvoke::EN:
	default:
		skillInvoked = "Invalid";
		break;
	}
	m_IsInvulnerable = true;
	m_InvulnerableTimer = 1.0f;
	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, skillInvoked);
#else
	m_IsInvulnerable = true;
	m_InvulnerableTimer = 1.0f;
#endif
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "Camera/CameraComponent.h" 
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include <Items/Weapon/Weapon.h>
#include <HUD/SlashHUD.h>
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "HUD/SlashOverlay.h"


// Sets default values
ASlashCharacter::ASlashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERABOOM"));
	cameraBoom->TargetArmLength = 400.0f;
	cameraBoom->SetupAttachment(GetRootComponent());

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	camera->SetupAttachment(cameraBoom);

}

// Called when the game starts or when spawned
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Cast the controller to a player controller
	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* system = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			system->AddMappingContext(characterContext, 0);
		}
	}
	
	Tags.Add(FName("EngageableTarget"));

	initializeSlashOverlay();

}

void ASlashCharacter::initializeSlashOverlay()
{
	APlayerController* controller = Cast<APlayerController>(GetController());
	if (controller) {
		ASlashHUD* slashHUD = Cast<ASlashHUD>(controller->GetHUD());
		if (slashHUD) {
			slashOverlay = Cast<USlashOverlay>(slashHUD->slashOverlay);
			if (slashOverlay) {
				slashOverlay->setGoldCount(0);
				slashOverlay->setSoulCount(0);
				slashOverlay->setHealthPercent(attributes->getPercentageHealth());
				slashOverlay->setStaminaPercent(1.0f);
			}
		}
	}
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (actionState != EActionState::Default) {
		return;
	}
	const float input = Value.Get<float>();
	if (Controller != nullptr) {
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, input);
	}
}

void ASlashCharacter::MoveRight(const FInputActionValue& Value)
{
	if (actionState != EActionState::Default) {
		return;
	}
	const float input = Value.Get<float>();
	if (Controller != nullptr) {
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, input);
	}
}

void ASlashCharacter::Turn(const FInputActionValue& Value)
{
	const FVector2D input = Value.Get<FVector2D>();
	if (Controller != nullptr) {
		AddControllerYawInput(input.X);
		AddControllerPitchInput(input.Y);

	}
}

void ASlashCharacter::Roll()
{
	if (actionState != EActionState::Default) {
		return;
	}

	if (attributes && attributes->stamina < attributes->staminaCost) {
		return;
	}

	actionState = EActionState::Roll;
	attributes->takeStamina(attributes->staminaCost);
	slashOverlay->setStaminaPercent(attributes->getPercentageStamina());

	playRoll();
}

void ASlashCharacter::rollEnd()
{
	actionState = EActionState::Default;
}

void ASlashCharacter::pressedE()
{

	if (actionState != EActionState::Default) {
		return;
	}

	AWeapon* weapon = Cast<AWeapon>(nearbyItem);
	if (weapon) {
		if (equippedWeapon) {
			characterState = ECharacterState::Unequipped;
			equippedWeapon->Destroy();
		}

		weapon->equip(GetMesh(), FName("Hand_rSocket"), this, this);
		characterState = ECharacterState::EquippedOneHandedWeapon;
		nearbyItem = nullptr;
		equippedWeapon = weapon;
	} else if (equippedWeapon)
	{
		if (isArmed()) {
			playDisarmed();
		}
		else if (isDisarmed()) {
			playArmed();
		}
	}
}

void ASlashCharacter::attack()
{
	if (canAttack()) {
		playAttack();
		actionState = EActionState::Attacking;
	}
}

bool ASlashCharacter::canAttack()
{
	return actionState == EActionState::Default && characterState == ECharacterState::EquippedOneHandedWeapon;
}

void ASlashCharacter::die_Implementation()
{
	Super::die_Implementation();
	actionState = EActionState::Dead;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASlashCharacter::playRoll()
{
	playMontageSection(rollMontage, FName("Default"));
}

void ASlashCharacter::playDisarmed()
{
	actionState = EActionState::Equipping;
	UAnimInstance* animation = GetMesh()->GetAnimInstance();
	if (animation && armMontage) {
		// need to set section
		animation->Montage_Play(armMontage);
		animation->Montage_JumpToSection(FName("Disarm"), armMontage);
	}
	characterState = ECharacterState::Unequipped;
}

void ASlashCharacter::disarmedCallback()
{
	equippedWeapon->equip(GetMesh(), FName("EquipSocket"), this, this);
}

void ASlashCharacter::playArmed()
{
	actionState = EActionState::Equipping;
	UAnimInstance* animation = GetMesh()->GetAnimInstance();
	if (animation && armMontage) {
		// need to set section
		animation->Montage_Play(armMontage);
		animation->Montage_JumpToSection(FName("Arm"), armMontage);
	}
	characterState = ECharacterState::EquippedOneHandedWeapon;
}

void ASlashCharacter::armedCallback()
{
	equippedWeapon->equip(GetMesh(), FName("Hand_rSocket"), this, this);
}

void ASlashCharacter::attackEndCallback()
{
	actionState = EActionState::Default;
}

void ASlashCharacter::equippedCallback()
{
	actionState = EActionState::Default;
}


bool ASlashCharacter::isArmed()
{
	return characterState == ECharacterState::EquippedOneHandedWeapon;
}

bool ASlashCharacter::isDisarmed()
{
	return characterState == ECharacterState::Unequipped;
}

int32 ASlashCharacter::playAttack()
{
	// Get the animation montage and animation instance
	UAnimInstance* animation = GetMesh()->GetAnimInstance();
	if (animation && attackMontage) {
		animation->Montage_Play(attackMontage);
	}
	return 0;
}

void ASlashCharacter::jump()
{
	if (actionState != EActionState::Default) {
		return;
	}
	Jump();
}

void ASlashCharacter::GetHit(const FHitResult& result, AActor* hitter)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Are we calling get hit?")));
	Super::GetHit(result, hitter);
	if (attributes && attributes->isAlive())
	{
		actionState = EActionState::HitReaction;
	}
	setBoxCollision(ECollisionEnabled::NoCollision);
}


void ASlashCharacter::GetHitCallback()
{
	actionState = EActionState::Default;
}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::takeDamage(DamageAmount);
	setHealthPercent();
	return DamageAmount;
}

void ASlashCharacter::setHealthPercent()
{
	if (attributes && slashOverlay) {
		slashOverlay->setHealthPercent(attributes->getPercentageHealth());
	}
}

// Called every frame
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (attributes && slashOverlay) {
		attributes->regenStamina(DeltaTime);
		slashOverlay->setStaminaPercent(attributes->getPercentageStamina());
	}

}

// Called to bind functionality to input
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ASlashCharacter::MoveRight);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Turn);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::jump);
		EnhancedInputComponent->BindAction(EAction, ETriggerEvent::Triggered, this, &ASlashCharacter::pressedE);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::attack);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Roll);
	}
}

void ASlashCharacter::setNearbyItem(AItem* Item)
{
	nearbyItem = Item;
}

void ASlashCharacter::pickUpSoul(ASoul* soul)
{
	if (attributes && slashOverlay) {
		attributes->soul += soul->soul;
		slashOverlay->setSoulCount(attributes->soul);
	}

}

void ASlashCharacter::pickUpGold(ATreasure* treasure)
{
	if (attributes && slashOverlay) {
		attributes->gold += treasure->gold;
		slashOverlay->setGoldCount(attributes->gold);
	}
}



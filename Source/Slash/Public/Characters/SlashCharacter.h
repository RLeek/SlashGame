// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "BaseCharacter.h"
#include "../interfaces/PickupInterface.h"
#include "GameStates.h"
#include "SlashCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class AWeapon;
class USlashOverlay;
class ASoul;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void setNearbyItem(AItem* Item) override;

	virtual void pickUpSoul(ASoul* soul) override;

	virtual void pickUpGold(ATreasure* treasure) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State)
	ECharacterState characterState = ECharacterState::Unequipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State)
	EActionState actionState = EActionState::Default;


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void initializeSlashOverlay();

	virtual void Move(const FInputActionValue& Value);

	virtual void MoveRight(const FInputActionValue& Value);

	virtual void Turn(const FInputActionValue& value);

	virtual void Roll();

	UFUNCTION(BlueprintCallable)
	virtual void rollEnd();

	virtual void pressedE();

	virtual void attack() override;

	virtual bool canAttack() override;

	virtual void die_Implementation() override;

	void playRoll();

	virtual void playDisarmed();

	UFUNCTION(BlueprintCallable)
	virtual void disarmedCallback();

	virtual void playArmed();

	UFUNCTION(BlueprintCallable)
	virtual void armedCallback();

	virtual void attackEndCallback() override;

	UFUNCTION(BlueprintCallable)
	virtual void equippedCallback();

	virtual bool isArmed();

	virtual bool isDisarmed();

	virtual int32 playAttack() override;

	virtual void jump();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* characterContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* RollAction;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* cameraBoom;
	 
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere)
	AItem* nearbyItem;

	UPROPERTY(EditAnywhere, Category = Movement)
	UAnimMontage* armMontage;

	UPROPERTY(EditAnywhere, Category = Movement)
	UAnimMontage* rollMontage;

	virtual void GetHit(const FHitResult& result, AActor* hitter) override;

	UFUNCTION(BlueprintCallable)
	void GetHitCallback();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void setHealthPercent();

	USlashOverlay* slashOverlay;

};

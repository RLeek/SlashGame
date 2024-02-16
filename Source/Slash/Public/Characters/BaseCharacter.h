// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../interfaces/HitInterface.h"
#include "GameFramework/Character.h"
#include "GameStates.h"
#include "BaseCharacter.generated.h"


class AWeapon;
class UAnimMontage;
class UAttributeComponent;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Related to weapon

	UFUNCTION(BlueprintCallable)
	virtual void setBoxCollision(ECollisionEnabled::Type collision);

	UPROPERTY(VisibleAnywhere, Category = State);
	AWeapon* equippedWeapon;

	// Related to attacking

	virtual void attack();

	UPROPERTY(BlueprintReadOnly)
	AActor* combatTarget;

	virtual int32 playAttack();

	virtual int32 playDeath();

	void stopAttack();

	UPROPERTY(EditAnywhere)
	double warpTarget = 75;

	UFUNCTION(BlueprintCallable)
	FVector getWarpTargetTranslation();

	UFUNCTION(BlueprintCallable)
	FVector getWarpTargetRotation();

	virtual bool canAttack();

	UFUNCTION(BlueprintCallable)
	virtual void attackEndCallback();

	UPROPERTY(EditAnywhere, Category = Movement)
	UAnimMontage* attackMontage;

	// Recieving damage
	UFUNCTION(BlueprintNativeEvent)
	void die();

	virtual void playHit(FName name);

	UPROPERTY(EditAnywhere, Category = Movement)
	UAnimMontage* hitMontage;

	virtual void GetHit(const FHitResult& result, AActor* hitter) override;

	void DetermineHitDirection(const FVector& result);

	UPROPERTY(EditAnywhere)
	UAttributeComponent* attributes;

	UPROPERTY(EditAnywhere, Category = Movement)
	UAnimMontage* deathMontage;

	UPROPERTY(EditAnywhere, Category = Particle)
	UParticleSystem* hitParticle;
	
	// play the correct montage
	// then we wanted to choose the correct montage

	void playMontageSection(UAnimMontage* Montage, const FName& SectionName);

	UPROPERTY(EditAnywhere)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere)
	TArray<FName> DeathMontageSections;


	int32 playRandomMontageSection(UAnimMontage* Montage, TArray<FName>& MontageSections);

	void DisableCapsule();

	bool IsAlive();

	void playParticles(FVector impactPoint);

	virtual void takeDamage(float DamageAmount);

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathState> state;

};

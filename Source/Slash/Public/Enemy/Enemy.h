// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Characters/BaseCharacter.h"
#include "../Characters/GameStates.h"
#include "Enemy.generated.h"


class UHealthBarWidget;
class UWidgetComponent;
class AAIController;
class UPawnSensingComponent;
class ASoul;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeEnemy();

	void SpawnDefaultWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void chasingAction();


	virtual void attack() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FHitResult& result, AActor* hitter) override;


	UPROPERTY(EditAnywhere)
	UHealthBarWidget* widget;

	virtual void attackEndCallback() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void die_Implementation() override;

	UPROPERTY(EditAnywhere)
	float deathSpan = 3.0f;


	UPROPERTY(EditAnywhere)
	int combatRadius = 700;


	UPROPERTY(EditAnywhere)
	int patrolRadius = 500;

	UPROPERTY(EditInstanceOnly)
	AActor* patrolTarget;

	UPROPERTY(EditAnywhere)
	int attackRadius = 200;

	UPROPERTY(EditAnywhere)
	float acceptanceRadius = 200;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> patrolTargets;

	AAIController* aiController;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* sensingComponent;

	UPROPERTY(BlueprintReadOnly)
	EEnemyAttackState enemyAttackState = EEnemyAttackState::Patrolling;

	bool inTargetRange(AActor* target, int radius);
	void moveToTarget(AActor* target);
	void selectNewTarget();

	UFUNCTION()
	void pawnSeen(APawn* seenPawn);

	FTimerHandle PatrolTimer;
	void finishTimerCallback();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASoul> SoulClass;

	void spawnSoul();

	/* AI */
	bool IsPatrolling();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();

	void hideHealthbar();
	void showHealthbar();
	void loseInterest();
	void startPatrolling();
	void startChasing();

	bool inCombatRadius();
	bool inAttackRadius();

	void clearPatrolTimer();


	/* Combat */
	void startAttacking();
	void clearAttackTimer();

	virtual bool canAttack() override;


	FTimerHandle attackTimer;

	UPROPERTY(EditAnywhere)
	float attackMin = 0.5f;

	UPROPERTY(EditAnywhere)
	float attackMax = 1.0f;

	UPROPERTY(EditAnywhere)
	float patrollingSpeed = 200.0f;

	UPROPERTY(EditAnywhere)
	float chasingSpeed = 450.0f;


	virtual void takeDamage(float DamageAmount) override;



};

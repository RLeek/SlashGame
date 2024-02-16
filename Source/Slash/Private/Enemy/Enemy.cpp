#include "Enemy/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Components/WidgetComponent.h" 
#include "GameFramework/CharacterMovementComponent.h" 
#include "Perception/PawnSensingComponent.h" 
#include "HUD/HealthBarWidget.h"
#include <Items/Weapon/Weapon.h>
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "Items/Soul.h"
#include "AIController.h" 

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


	widget = CreateDefaultSubobject<UHealthBarWidget>(TEXT("Healthbar"));
	widget->SetupAttachment(GetRootComponent());

	sensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	sensingComponent->SetPeripheralVisionAngle(45.f);
	sensingComponent->SightRadius = 400.f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (sensingComponent) sensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::pawnSeen);

	InitializeEnemy();
	Tags.Add(FName("Enemy"));
}

void AEnemy::InitializeEnemy()
{
	aiController = Cast<AAIController>(GetController());
	moveToTarget(patrolTarget);
	hideHealthbar();
	SpawnDefaultWeapon();
}

void AEnemy::SpawnDefaultWeapon()
{
	if (GetWorld() && WeaponClass) {
		AWeapon* weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		weapon->equip(GetMesh(), FName("WeaponSocket"), this, this);
		equippedWeapon = weapon;
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;
	if (!IsPatrolling() && !IsEngaged()) {
		chasingAction();
	}

	if (IsPatrolling()) {
		if (patrolTarget) {
			// We are at target
			if (inTargetRange(patrolTarget, patrolRadius)) {
				selectNewTarget();
				// Use the timer to move to the new location
				GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::finishTimerCallback, 5.f);
			}
		}
	}
}

void AEnemy::chasingAction()
{
	// Get actor location compare distance and radius and hide widget
	if (combatTarget) {
		if (!inCombatRadius()) {
			clearAttackTimer();
			loseInterest();
			if (!IsEngaged()) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Do we not start patrolling"));
				startPatrolling();
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Patrolling"));
		} else if (!inAttackRadius() && !IsChasing()) {
			clearPatrolTimer();
			clearAttackTimer();
			if (!IsEngaged()) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Do we not start Chasing?"));
				startChasing();
				showHealthbar();
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Chasing"));
		} else if (canAttack()) {
			clearPatrolTimer();
			showHealthbar();
			startAttacking();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attacking"));
		}
	}
}

void AEnemy::attack()
{
	Super::attack();
	if (combatTarget == nullptr) {
		startPatrolling();
		return;
	}
	enemyAttackState = EEnemyAttackState::Engaged;
	playAttack();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	chasingAction();
}

void AEnemy::GetHit(const FHitResult& result, AActor* hitter)
{
	Super::GetHit(result, hitter);
	if (!IsDead()) {
		showHealthbar();
	}
	clearPatrolTimer();
	clearAttackTimer();
	
	setBoxCollision(ECollisionEnabled::NoCollision);
	stopAttack();
	if (inAttackRadius()) {
		if (!IsDead()) startAttacking();
	}
}

void AEnemy::attackEndCallback()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("We get called"));
	enemyAttackState = EEnemyAttackState::NoState;
	chasingAction();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	takeDamage(DamageAmount);
	clearPatrolTimer();
	combatTarget = EventInstigator->GetPawn();
	if (inAttackRadius()) {
		enemyAttackState = EEnemyAttackState::Attacking;
	}
	else if (!inAttackRadius()) {
		startChasing();
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Recieved attack, now chasing"));
	return DamageAmount;
}

void AEnemy::takeDamage(float DamageAmount) 
{
	Super::ABaseCharacter::takeDamage(DamageAmount);
	if (widget) {
		widget->setHealthPercent(attributes->getPercentageHealth());
	}
}

void AEnemy::die_Implementation()	
{
 	clearAttackTimer();
	clearPatrolTimer();
	Super::playDeath();
	enemyAttackState = EEnemyAttackState::Dead;
	hideHealthbar();
	equippedWeapon->Destroy();
	DisableCapsule();
	SetLifeSpan(deathSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	spawnSoul();
}

bool AEnemy::inTargetRange(AActor* target, int radius)
{
	int32 distance = (target->GetActorLocation() - GetActorLocation()).Size();
	if (distance <= radius) {
		return true;
	}
	return false;
}

void AEnemy::moveToTarget(AActor* target)
{
	if (aiController == nullptr || target == nullptr) return;
	FAIMoveRequest moveRequest;
	moveRequest.SetGoalActor(target);
	moveRequest.SetAcceptanceRadius(acceptanceRadius);
	aiController->MoveTo(moveRequest);
}

void AEnemy::finishTimerCallback()
{
	moveToTarget(patrolTarget);
}

void AEnemy::spawnSoul()
{

	UWorld* world = GetWorld();
	if (SoulClass && world && attributes) {
		const FVector spawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.0f);
		ASoul* spawnedSoul = world->SpawnActor<ASoul>(SoulClass, spawnLocation, GetActorRotation());
		spawnedSoul->SetOwner(this);
		spawnedSoul->soul = attributes->soul;
	}
}

bool AEnemy::IsPatrolling()
{
	return enemyAttackState == EEnemyAttackState::Patrolling;
}

bool AEnemy::IsChasing()
{
	return enemyAttackState == EEnemyAttackState::Chasing;
}

bool AEnemy::IsAttacking()
{
	return enemyAttackState == EEnemyAttackState::Attacking;
}

bool AEnemy::IsDead()
{
	return enemyAttackState == EEnemyAttackState::Dead;
}

void AEnemy::loseInterest()
{
	combatTarget = nullptr;
	hideHealthbar();
}

bool AEnemy::IsEngaged()
{
	return enemyAttackState == EEnemyAttackState::Engaged;
}

void AEnemy::hideHealthbar()
{
	if (widget) {
		widget->SetVisibleFlag(false);
	}
}

void AEnemy::showHealthbar()
{
	if (widget) {
		widget->SetVisibleFlag(true);
	}
}

void AEnemy::startPatrolling()
{
	enemyAttackState = EEnemyAttackState::Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = patrollingSpeed;
	moveToTarget(patrolTarget);
}

void AEnemy::startChasing()
{
	enemyAttackState = EEnemyAttackState::Chasing;
	GetCharacterMovement()->MaxWalkSpeed = chasingSpeed;
	moveToTarget(combatTarget);
}

bool AEnemy::inCombatRadius()
{
	return inTargetRange(combatTarget, combatRadius);
}

bool AEnemy::inAttackRadius()
{
	return inTargetRange(combatTarget, attackRadius);
}

void AEnemy::clearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::startAttacking()
{
	enemyAttackState = EEnemyAttackState::Attacking;
	const float attackTime = FMath::RandRange(attackMin, attackMax);
	GetWorldTimerManager().SetTimer(attackTimer, this, &AEnemy::attack, attackTime);

}

void AEnemy::clearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(attackTimer);
}

bool AEnemy::canAttack()
{
	return inAttackRadius() && !IsAttacking() && !IsDead() && !IsEngaged();
}

void AEnemy::selectNewTarget()
{
	TArray<AActor*> validTargets;
	for (AActor* target : patrolTargets) {
		if (target != patrolTarget) {
			validTargets.AddUnique(target);
		}
	}
	if (validTargets.Num() > 0) {
		int target = FMath::RandRange(0, validTargets.Num() - 1);
		patrolTarget = patrolTargets[target];
	}
}

void AEnemy::pawnSeen(APawn* seenPawn)
{
	const bool ShouldChaseTarget =
		enemyAttackState != EEnemyAttackState::Chasing &&
		enemyAttackState != EEnemyAttackState::Attacking &&
		seenPawn->ActorHasTag(FName("EngageableTarget")) &&
		enemyAttackState < EEnemyAttackState::Attacking;

	if (ShouldChaseTarget) {
		combatTarget = seenPawn;
		clearPatrolTimer();
		startChasing();
		showHealthbar();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("See player, now chasing"));
	}
}


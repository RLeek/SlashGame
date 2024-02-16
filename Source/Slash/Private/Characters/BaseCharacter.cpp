// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h" 
#include <Items/Weapon/Weapon.h>
#include "Kismet/KismetSystemLibrary.h" 
#include "Components/CapsuleComponent.h" 
#include "Components/AttributeComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Characters/GameStates.h>

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::setBoxCollision(ECollisionEnabled::Type collision)
{
	if (equippedWeapon) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("This message will appear on the screen!"));
		UBoxComponent* weaponBox = equippedWeapon->GetWeaponBox();
		weaponBox->SetCollisionEnabled(collision);
		equippedWeapon->FixedActorsToIgnore.Empty();
	}
}

void ABaseCharacter::attack()
{
	if (combatTarget && combatTarget->ActorHasTag(FName("Dead"))) {
		combatTarget = nullptr;
	}
}

int32 ABaseCharacter::playAttack()
{
	return playRandomMontageSection(attackMontage, AttackMontageSections);
}

int32 ABaseCharacter::playDeath()
{
	const int32 selection = playRandomMontageSection(deathMontage, DeathMontageSections);
	TEnumAsByte<EDeathState> Pose(selection);
	if (Pose < EDeathState::MAX) {
		state = Pose;
	}
	return selection;
}

void ABaseCharacter::stopAttack()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	if (animInstance && attackMontage) {
		animInstance->Montage_Stop(0.25, attackMontage);
	}
}

FVector ABaseCharacter::getWarpTargetTranslation()
{
	const FVector CharaLocation = GetActorLocation();
	const FVector targetLocation = combatTarget->GetActorLocation();

	FVector direction = (CharaLocation - targetLocation).GetSafeNormal();
	direction *= warpTarget;
	return direction + combatTarget->GetActorLocation();
}

FVector ABaseCharacter::getWarpTargetRotation()
{
	if (combatTarget) {
		return combatTarget->GetActorLocation();
	}

	return FVector();
}

bool ABaseCharacter::canAttack()
{
	return false;
}

void ABaseCharacter::attackEndCallback()
{
}

void ABaseCharacter::die_Implementation()
{
	Tags.Add(FName("Dead"));
	playDeath();
}

void ABaseCharacter::playHit(FName name)
{
	UAnimInstance* animation = GetMesh()->GetAnimInstance();
	if (animation && hitMontage) {
		// need to set section
		animation->Montage_Play(hitMontage);
		animation->Montage_JumpToSection(name, hitMontage);
	}
}

void ABaseCharacter::GetHit(const FHitResult& result, AActor* hitter)
{
	if (IsAlive()) {
		DetermineHitDirection(hitter->GetActorLocation());
	}
	else {
		die();
	}
	playParticles(result.ImpactPoint);
}

void ABaseCharacter::DetermineHitDirection(const FVector& result)
{
	FVector normalizedImpactPont = FVector(result.X, result.Y, GetActorLocation().Z);
	FVector actorHit = (normalizedImpactPont - GetActorLocation()).GetSafeNormal();
	FVector actorForward = GetActorForwardVector();

	double cosTheta = FVector::DotProduct(actorHit, actorForward);
	double theta = FMath::Acos(cosTheta);
	theta = FMath::RadiansToDegrees(theta);

	const FVector crossProduct = FVector::CrossProduct(actorForward, actorHit);
	if (crossProduct.Z < 0) {
		theta *= -1.f;
	}

	FName section = FName("HitBack");
	if (theta >= -45.f && theta < 45.f) {
		section = FName("HitFront");
	}
	else if (theta >= -135.f && theta < -45.f) {
		section = FName("HitLeft");
	}
	else if (theta >= 45.f && theta <= 135.f) {
		section = FName("HitRight");
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Theta: %f"), theta));
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + actorHit * 50, 1, FLinearColor::Red, 5.0f, 1.0f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + actorForward * 50, 1, FLinearColor::Green, 5.0f, 1.0f);
	playHit(section);
}

void ABaseCharacter::playMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* animation = GetMesh()->GetAnimInstance();
	if (animation && Montage) {
		animation->Montage_Play(Montage);
		animation->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::playRandomMontageSection(UAnimMontage* Montage, TArray<FName>& MontageSections)
{
	if (MontageSections.Num() <= 0) return -1;
	int sectionName = FMath::RandRange(0, MontageSections.Num() - 1);
	playMontageSection(Montage, MontageSections[sectionName]);
	return sectionName;
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ABaseCharacter::IsAlive()
{
	return attributes && attributes->isAlive();
}

void ABaseCharacter::playParticles(FVector impactPoint)
{
	if (hitParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			hitParticle,
			impactPoint
		);
	}
}

void ABaseCharacter::takeDamage(float DamageAmount)
{
	if (attributes) 
	{
		attributes->takeDamage(DamageAmount);
	}
}

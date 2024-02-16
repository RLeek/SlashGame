// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacterAnimationInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/KismetMathLibrary.h"


void USlashCharacterAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	slashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (slashCharacter) {
		slashMovementComp = slashCharacter->GetCharacterMovement();
	}
}

void USlashCharacterAnimationInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (slashMovementComp) {
		groundSpeed = slashMovementComp->Velocity.Size2D();
		isFalling = slashMovementComp->IsFalling();
		characterState = slashCharacter->characterState;
		deathState = slashCharacter->state;
		actionState = slashCharacter->actionState;
	}
}

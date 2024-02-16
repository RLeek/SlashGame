// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SlashCharacter.h"
#include "SlashCharacterAnimationInstance.generated.h"

class ASlashCharacter;
class UCharacterMovementComponent;

UCLASS()
class SLASH_API USlashCharacterAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	ECharacterState characterState = ECharacterState::Unequipped;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float groundSpeed;

	UPROPERTY(BlueprintReadOnly)
	ASlashCharacter* slashCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* slashMovementComp;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EActionState actionState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDeathState> deathState;

};

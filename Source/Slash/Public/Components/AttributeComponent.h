// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	float Health = 100;

	UPROPERTY(VisibleAnywhere)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float stamina = 120;

	UPROPERTY(VisibleAnywhere)
	float MaxStamina = 120;

	UPROPERTY(EditAnywhere)
	int32 gold = 0;

	UPROPERTY(EditAnywhere)
	int32 soul = 0;

	UPROPERTY(EditAnywhere)
	int32 staminaCost = 14;

	UPROPERTY(EditAnywhere)
	int32 staminaRegen = 2;

	void takeDamage(float damage);

	void takeStamina(float cost);

	void regenStamina(float deltaTime);

	float getPercentageStamina();

	float getPercentageHealth();

	bool isAlive();
		
};

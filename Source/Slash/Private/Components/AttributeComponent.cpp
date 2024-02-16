// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::takeDamage(float damage)
{
	Health = FMath::Clamp(Health - damage, 0, MaxHealth);
}

void UAttributeComponent::takeStamina(float cost)
{
	stamina = FMath::Clamp(stamina - cost, 0, MaxStamina);
}

void UAttributeComponent::regenStamina(float deltaTime)
{
	stamina = FMath::Clamp(stamina + staminaRegen * deltaTime, 0.f, MaxStamina);
}

float UAttributeComponent::getPercentageStamina()
{
	return stamina / MaxStamina;
}

float UAttributeComponent::getPercentageHealth()
{
	return Health / MaxHealth;
}

bool UAttributeComponent::isAlive()
{
	if (Health <= 0) {
		return false;
	}
	return true;
}




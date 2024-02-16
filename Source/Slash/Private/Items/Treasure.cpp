// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include <Characters/SlashCharacter.h>

void ATreasure::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* character = Cast<IPickupInterface>(OtherActor);
	if (character) {
		character->pickUpGold(this);
		Destroy();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Kismet/KismetSystemLibrary.h" 
#include <interfaces/PickupInterface.h>

void ASoul::BeginPlay()
{
	Super::BeginPlay();
	const FVector start = GetActorLocation();
	const FVector end = start - FVector(0, 0, 1000);

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		start,
		end,
		objectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::Persistent,
		HitResult,
		true
	);
	desiredZ = HitResult.ImpactPoint.Z + 100.f;
}

void ASoul::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
	if (GetActorLocation().Z > desiredZ) {
		AddActorWorldOffset(FVector(0, 0, dropRate * Deltatime));
	}

}

void ASoul::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* character = Cast<IPickupInterface>(OtherActor);
	if (character) {
		character->pickUpSoul(this);
		playPickUpEffect();
		Destroy();
	}
}


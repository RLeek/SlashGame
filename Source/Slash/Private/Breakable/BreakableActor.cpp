// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h" 
#include <Items/Treasure.h>
#include <Components/CapsuleComponent.h>

// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Component"));
	RootComponent = GeometryCollection;
	GeometryCollection->SetGenerateOverlapEvents(true);

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	capsule->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit(const FHitResult& result, AActor* hitter)
{
	if (GetWorld() && treasures.Num() > 0) {

		FVector Location = GetActorLocation();
		Location.Z += 50;
		int32 index = FMath::RandRange(0, treasures.Num() - 1);
		GetWorld()->SpawnActor<ATreasure>(treasures[index], Location, GetActorRotation());
	}

	SetLifeSpan(3);

}


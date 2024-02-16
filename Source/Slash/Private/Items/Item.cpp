#include "Items/Item.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h" 
#include "Slash/DebugMacros.h"
#include "NiagaraComponent.h" 
#include <Characters/SlashCharacter.h>
#include <NiagaraFunctionLibrary.h>


AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	itemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Internal Item Mesh"));
	itemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	itemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RootComponent = itemMesh;

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	sphere->SetupAttachment(GetRootComponent());

	niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	niagara->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::onSphereOverlap);
	sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::onSphereLeaveOverlap);

	//DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_SixParams(FComponentBeginOverlapSignature, UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult);

}

float AItem::TransformedSin(float value)
{
	return amplitude * FMath::Sin(value * period);
}

float AItem::TransformedCos(float value)
{
	return amplitude * FMath::Cos(value * period);
}

void AItem::onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface *character = Cast<IPickupInterface>(OtherActor);
	if (character) {
		character->setNearbyItem(this);
	}

}

void AItem::onSphereLeaveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* character = Cast<IPickupInterface>(OtherActor);
	if (character) {
		character->setNearbyItem(nullptr);
	}

}

void AItem::playPickUpEffect()
{
	if (pickUpEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, pickUpEffect, GetActorLocation());
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	time += DeltaTime;

	if (itemState == EItemState::hovering) {
		float zMov = TransformedSin(time);
		AddActorWorldOffset(FVector(0, 0, zMov));
	}
	
	DRAW_SPHERE_single_frame(GetActorLocation());
	/*
	DRAW_VECTOR_single_frame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.0f);

	DRAW_POINT_single_frame(avg(GetActorLocation(), FVector::ZeroVector))
	*/
}


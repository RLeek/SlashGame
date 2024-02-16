// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h" 
#include "Kismet/KismetSystemLibrary.h" 
#include "Components/BoxComponent.h" 
#include "NiagaraComponent.h" 
#include "Components/SphereComponent.h" 
#include <interfaces/HitInterface.h>

AWeapon::AWeapon()
{
	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	collisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	collisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	collisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	collisionBox->SetupAttachment(GetRootComponent());

	traceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	traceStart->SetupAttachment(collisionBox);

	traceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	traceEnd->SetupAttachment(collisionBox);
}

void AWeapon::equip(USceneComponent* InParent, FName InSocketName, APawn* instigator, AActor* owner)
{
	itemState = EItemState::equpped;
	SetOwner(owner);
	SetInstigator(instigator);
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
	itemMesh->AttachToComponent(InParent, rules, InSocketName);
	DisableSphere();
	DisableNiagara();
}

void AWeapon::DisableSphere() 
{
	if (sphere) {
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::DisableNiagara()
{
	if (niagara)
	{
		niagara->Deactivate();
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	FVector start = traceStart->GetComponentLocation();
	FVector end = traceEnd->GetComponentLocation();

	TArray<AActor*>  ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* actor : FixedActorsToIgnore) {
		ActorsToIgnore.AddUnique(actor);
	}
	UKismetSystemLibrary::BoxTraceSingle(
		this, start, end, BoxTraceExtent, traceStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, BoxHit, true
	);

	FixedActorsToIgnore.AddUnique(BoxHit.GetActor());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::onBoxOverlap);
}

void AWeapon::onBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"))) {
		return;
	}

	FHitResult OutHit;
	BoxTrace(OutHit);
	if (OutHit.GetActor() && OutHit.GetActor()->ActorHasTag(TEXT("Enemy")) && GetOwner()->ActorHasTag(TEXT("Enemy"))) {
		return;
	}

	IHitInterface* hitObject = Cast<IHitInterface>(OutHit.GetActor());
	if (hitObject) {
		UGameplayStatics::ApplyDamage(
			OutHit.GetActor(),
			damage,
			GetInstigator()->GetController(),
			GetOwner(),
			UDamageType::StaticClass()
		);
		hitObject->GetHit(OutHit, GetOwner());
	}

	createFields(OutHit.ImpactPoint);
}

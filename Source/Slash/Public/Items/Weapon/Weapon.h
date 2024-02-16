// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"


class UBoxComponent;
/**
 * 
 */
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()


public:
	AWeapon();
	void equip(USceneComponent *InParent, FName InSocketName, APawn* instigator, AActor* owner);

	FORCEINLINE UBoxComponent* GetWeaponBox() const { return collisionBox; };

	TArray<AActor*> FixedActorsToIgnore;

protected:
	virtual void BeginPlay() override;

	void DisableSphere();

	void DisableNiagara();

	UFUNCTION(BlueprintImplementableEvent)
	void createFields(const FVector& fieldLocation);

	UFUNCTION()
	void onBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere);
	UBoxComponent* collisionBox;

	UPROPERTY(VisibleAnywhere);
	USceneComponent* traceStart;

	UPROPERTY(VisibleAnywhere);
	USceneComponent* traceEnd;

	UPROPERTY(VisibleAnywhere)
	float damage = 20;

	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere)
	FVector BoxTraceExtent = FVector(5.0f);

	UPROPERTY(EditAnywhere)
	bool bShowBoxDebug = true;




};

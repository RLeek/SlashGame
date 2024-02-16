// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

enum class EItemState {
	hovering,
	equpped
};


UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

	EItemState itemState = EItemState::hovering;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sine Parameters");
	float amplitude = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sine Parameters");
	float period =  2.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin(float value);

	UFUNCTION(BlueprintPure)
	float TransformedCos(float value);

	UFUNCTION()
	virtual void onSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void onSphereLeaveOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	template<typename T>
	T avg(T value1, T value2);

	UPROPERTY(VisibleAnywhere);
	UStaticMeshComponent* itemMesh;

	UPROPERTY(VisibleAnywhere);
	USphereComponent* sphere;

	UPROPERTY(VisibleAnywhere);
	UNiagaraComponent* niagara;

	UPROPERTY(EditAnywhere);
	UNiagaraSystem* pickUpEffect;

	void playPickUpEffect();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess="true"));
	float time = 0.f;
};

template<typename T>
inline T AItem::avg(T value1, T value2)
{
	return (value1 + value2)/2;
}

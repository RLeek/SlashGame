// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.generated.h"

class UHealthBar;


/**
 * 
 */
UCLASS()
class SLASH_API UHealthBarWidget : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void setHealthPercent(float percent);

private:

	UPROPERTY()
	UHealthBar* healthBar;

};

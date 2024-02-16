// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/HealthBarWidget.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h" 

void UHealthBarWidget::setHealthPercent(float percent)
{
	// Get the widget object
	if (!healthBar) {
		healthBar = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (healthBar) {
		healthBar->HealthBar->SetPercent(percent);
	}
}

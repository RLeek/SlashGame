// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h" 
#include "Components/TextBlock.h" 

void USlashOverlay::setStaminaPercent(float percent)
{
	if (stamina) {
		stamina->SetPercent(percent);
	}
}

void USlashOverlay::setHealthPercent(float percent)
{
	if (health) {
		health->SetPercent(percent);
	}

}

void USlashOverlay::setGoldCount(int32 gold)
{
	if (goldCount) {
		goldCount->SetText(FText::FromString(FString::Printf(TEXT("%d"),gold)));
	}
}

void USlashOverlay::setSoulCount(int32 soul)
{
	if (soulCount) {
		soulCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), soul)));
	}
}


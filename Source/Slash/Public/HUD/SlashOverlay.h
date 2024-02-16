// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashOverlay.generated.h"

/**
 * 
 */
class UProgressBar;
class UTextBlock;

UCLASS()
class SLASH_API USlashOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void setStaminaPercent(float percent);
	void setHealthPercent(float percent);

	void setGoldCount(int32 gold);
	void setSoulCount(int32 soul);

	UPROPERTY(meta = (BindWidget))
	UProgressBar* health;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* stamina;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* goldCount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* soulCount;
	
};

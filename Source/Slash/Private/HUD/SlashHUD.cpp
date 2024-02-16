// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/SlashOverlay.h"
#include "HUD/SlashHUD.h"

void ASlashHUD::BeginPlay()
{
	UWorld* world = GetWorld();
	if (world) {
		APlayerController* controller = world->GetFirstPlayerController();
		if (controller && slashOverlayClass) {
			slashOverlay = CreateWidget<USlashOverlay>(controller, slashOverlayClass);
			slashOverlay->AddToViewport();
		}
	}

}

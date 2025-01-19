// Fill out your copyright notice in the Description page of Project Settings.

#include "ToonTanksPlayerController.h"
#include "Blueprint/UserWidget.h"

void AToonTanksPlayerController::BeginPlay()
{
	if (!HUD) return;
	UUserWidget* HUDWidget = CreateWidget(this, HUD);
	HUDWidget->AddToViewport();
}

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}

	bShowMouseCursor = bPlayerEnabled;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/AfkUI.h"
#include "UI/MainUI.h"
#include "UI/UpgradeUI.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
	check(MainUI)
	MainUI->AddToViewport();

	AfkUI = CreateWidget<UAfkUI>(GetWorld(), AfkUIClass);
	check(AfkUI)
	AfkUI->AddToViewport();

	UpgradeUI = CreateWidget<UUpgradeUI>(GetWorld(), UpgradeUIClass);
	check(UpgradeUI)
	UpgradeUI->AddToViewport();
	UpgradeUI->SetVisibility(ESlateVisibility::Collapsed);

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

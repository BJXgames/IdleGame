// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "MainGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UI/AfkUI.h"
#include "UI/MainUI.h"
#include "UI/ManagerPanelUI.h"
#include "UI/UpgradeUI.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MainGameInstance = GetGameInstance<UMainGameInstance>();
	
	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
	check(MainUI)
	MainUI->AddToViewport();

	if(MainGameInstance->DataToLoad)
	{
		AfkUI = CreateWidget<UAfkUI>(GetWorld(), AfkUIClass);
		check(AfkUI)
		AfkUI->AddToViewport();
	}

	UpgradeUI = CreateWidget<UUpgradeUI>(GetWorld(), UpgradeUIClass);
	check(UpgradeUI)
	UpgradeUI->AddToViewport();
	UpgradeUI->SetVisibility(ESlateVisibility::Collapsed);

	ManagerPanelUI = CreateWidget<UManagerPanelUI>(GetWorld(), ManagerPanelUIClass);
	check(ManagerPanelUI)
	ManagerPanelUI->AddToViewport();
	ManagerPanelUI->SetVisibility(ESlateVisibility::Collapsed);
	

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

UManagerPanelUI* AMainPlayerController::GetManagerPanelUI() const
{
	return ManagerPanelUI;
}

UUpgradeUI* AMainPlayerController::GetUpgradeUI() const
{
	return UpgradeUI;
}



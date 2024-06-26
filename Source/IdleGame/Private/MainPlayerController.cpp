// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "MainGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UI/AfkUI.h"
#include "UI/MainUI.h"
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
	if (!UpgradeUI)
	{
		UE_LOG(LogTemp, Error, TEXT("UpgradeUI creation failed in AMainPlayerController::BeginPlay"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UpgradeUI created successfully in AMainPlayerController::BeginPlay"));
	}
	

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}


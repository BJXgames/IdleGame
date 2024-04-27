// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/AfkUI.h"
#include "UI/MainUI.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
	check(MainUI)
	MainUI->AddToViewport();

	AfkUI = CreateWidget<UAfkUI>(GetWorld(), AfkUIClass);
	check(AfkUI)
	AfkUI->AddToViewport();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ManagerUI.h"

#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "MainWorldSubsystem.h"
#include "Components/Border.h"
#include "UI/ManagerPanelUI.h"

void UManagerUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AMainPlayerController>(GetOwningPlayer());
	MainGameInstance = GetGameInstance<UMainGameInstance>();
	WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();

	FLinearColor CustomColor(.1, .1, .1, 0.0);
	ManagerImage->SetBrushColor(CustomColor);
}

void UManagerUI::OpenManagerPanel()
{
	PlayerController->ManagerPanelUI->SetVisibility(ESlateVisibility::Visible);
}

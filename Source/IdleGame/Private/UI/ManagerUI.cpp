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
	PlayerController->GetManagerPanelUI()->SetVisibility(ESlateVisibility::Visible);
}

void UManagerUI::UpdateManagerImage(UTexture2D* Image)
{
	if (ManagerImage)
	{
		if (Image)
		{
			FLinearColor TintColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
			ManagerImage->SetBrushFromTexture(Image);
			ManagerImage->SetBrushColor(TintColor);
		}
		else
		{
			FLinearColor TintColor = FLinearColor(1.f, 1.f, 1.f, 0.f);
			ManagerImage->SetBrushColor(TintColor);
		}
	}
	
}


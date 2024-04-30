// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UpgradeUI.h"

#include "MainGameInstance.h"
#include "MainWorldSubsystem.h"
#include "Components/TextBlock.h"

void UUpgradeUI::NativeConstruct()
{
	Super::NativeConstruct();
	WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();
	MainGameInstance = GetGameInstance<UMainGameInstance>();
	
	SpeedUpgradePriceText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(SpeedPrice)));
}

void UUpgradeUI::UpdateGenText(double Quantity, double MaxTime, double Income, FString GenName)
{
	GenIncomeText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(Income * (1 / MaxTime) * Quantity)));
	GenQuantityText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(Quantity)));
	GenSpeedText->SetText(FText::FromString(WorldSubsystem->FormatNumber(MaxTime) + "s"));
	GenNameText->SetText(FText::FromString(GenName));

	// UE_LOG(LogTemp, Warning, TEXT("speed: %s"), *WorldSubsystem->FormatLargeNumber(MaxTime))
}

void UUpgradeUI::UpgradeSpeed()
{
	if(MainGameInstance->Money >= SpeedPrice)
	{
		MainGameInstance->Money -= SpeedPrice;
		SpeedPrice *= 10;
		SpeedUpgradePriceText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(SpeedPrice)));
	}
	else
	{
		 return;
	}
}



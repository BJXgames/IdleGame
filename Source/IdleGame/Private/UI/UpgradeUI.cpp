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
}

void UUpgradeUI::UpdateGenText(double Quantity, double MaxTime, double Income, FString GenName)
{
	GenIncomeText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(Income * (1 / MaxTime) * Quantity)));
	GenQuantityText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(Quantity)));
	GenSpeedText->SetText(FText::FromString(WorldSubsystem->FormatNumber(MaxTime) + "s"));
	GenNameText->SetText(FText::FromString(GenName));
	SpeedUpgradePriceText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(CurrentGenerator->GeneratorData.SpeedPrice)));

	// UE_LOG(LogTemp, Warning, TEXT("speed: %s"), *WorldSubsystem->FormatLargeNumber(MaxTime))
}

void UUpgradeUI::UpgradeSpeed()
{
	if(CurrentGenerator->GeneratorData.MaxTime > 0.14)
	{
		if(MainGameInstance->Money >= CurrentGenerator->GeneratorData.SpeedPrice)
		{
			MainGameInstance->Money -= CurrentGenerator->GeneratorData.SpeedPrice;
			CurrentGenerator->GeneratorData.SpeedPrice *= 50;
			SpeedUpgradePriceText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(CurrentGenerator->GeneratorData.SpeedPrice)));
			CurrentGenerator->GeneratorData.MaxTime -= 0.05;
			UpdateGenText(CurrentGenerator->GeneratorData.Quantity, CurrentGenerator->GeneratorData.MaxTime, CurrentGenerator->GeneratorData.Income, CurrentGenerator->GeneratorData.GeneratorName);

			if(CurrentGenerator->GeneratorData.MaxTime < 0.14)
			{
				UpdateGenText(CurrentGenerator->GeneratorData.Quantity, CurrentGenerator->GeneratorData.MaxTime, CurrentGenerator->GeneratorData.Income, CurrentGenerator->GeneratorData.GeneratorName);
				SpeedUpgradePriceText->SetText(FText::FromString("Max"));
			}
		}
		else
		{
			if(CurrentGenerator->GeneratorData.MaxTime < 0.14)
			{
				UpdateGenText(CurrentGenerator->GeneratorData.Quantity, CurrentGenerator->GeneratorData.MaxTime, CurrentGenerator->GeneratorData.Income, CurrentGenerator->GeneratorData.GeneratorName);
				SpeedUpgradePriceText->SetText(FText::FromString("Max"));
			}
			UpdateGenText(CurrentGenerator->GeneratorData.Quantity, CurrentGenerator->GeneratorData.MaxTime, CurrentGenerator->GeneratorData.Income, CurrentGenerator->GeneratorData.GeneratorName);
		}
	}
	else
	{
		UpdateGenText(CurrentGenerator->GeneratorData.Quantity, CurrentGenerator->GeneratorData.MaxTime, CurrentGenerator->GeneratorData.Income, CurrentGenerator->GeneratorData.GeneratorName);
		SpeedUpgradePriceText->SetText(FText::FromString("Max"));
	}
	
}



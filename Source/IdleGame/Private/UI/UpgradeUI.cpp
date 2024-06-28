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

void UUpgradeUI::UpdateGenText(FLargeNumber Quantity, FLargeNumber MaxTime, FLargeNumber Income, FString GenName)
{
	Income = Income * (1 / MaxTime.Value) * Quantity;
	Income.Normalize();

	Quantity.Normalize();
	MaxTime.Normalize();

	CurrentGenerator->GeneratorData.SpeedPrice.Normalize();
	
	GenIncomeText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(Income)));
	GenQuantityText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(Quantity)));
	GenSpeedText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(MaxTime) + "s"));
	GenNameText->SetText(FText::FromString(GenName));
	SpeedUpgradePriceText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(CurrentGenerator->GeneratorData.SpeedPrice)));

	const float MinMaxTime = 0.14f; // Minimum max time threshold

	if (CurrentGenerator->GeneratorData.MaxTime.Value < MinMaxTime)
	{
		SpeedUpgradePriceText->SetText(FText::FromString("Max"));
	}
	else
	{
		SpeedUpgradePriceText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(CurrentGenerator->GeneratorData.SpeedPrice)));
	}
	

	// UE_LOG(LogTemp, Warning, TEXT("speed: %s"), *WorldSubsystem->FormatLargeNumber(MaxTime))
}

void UUpgradeUI::UpgradeSpeed()
{
    const float MinMaxTime = 0.14f; // Minimum max time threshold
    const float MaxTimeReduction = 0.1f; // Amount to reduce max time by
    const float SpeedPriceMultiplier = 50.0f; // Multiplier for speed price

    if (CurrentGenerator->GeneratorData.MaxTime.Value > MinMaxTime)
    {
        if (MainGameInstance->Money >= CurrentGenerator->GeneratorData.SpeedPrice)
        {
            // Deduct the speed upgrade cost
            MainGameInstance->Money -= CurrentGenerator->GeneratorData.SpeedPrice;

            // Increase the speed upgrade price for the next upgrade
            CurrentGenerator->GeneratorData.SpeedPrice.Value *= SpeedPriceMultiplier;
            SpeedUpgradePriceText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(CurrentGenerator->GeneratorData.SpeedPrice)));

            // Reduce the max time for the generator
            CurrentGenerator->GeneratorData.MaxTime.Value -= MaxTimeReduction;

            // Update the generator text
            UpdateGenText(CurrentGenerator->GeneratorData.Quantity.Value, CurrentGenerator->GeneratorData.MaxTime.Value, CurrentGenerator->GeneratorData.Income.Value, CurrentGenerator->GeneratorData.GeneratorName);

            // Check if the max time is below the minimum threshold
            if (CurrentGenerator->GeneratorData.MaxTime.Value < MinMaxTime)
            {
                SpeedUpgradePriceText->SetText(FText::FromString("Max"));
            }
        }
        else
        {
            // Insufficient money for upgrade, but check if max time is below threshold
            if (CurrentGenerator->GeneratorData.MaxTime.Value < MinMaxTime)
            {
                SpeedUpgradePriceText->SetText(FText::FromString("Max"));
            }

            // Update the generator text regardless of the condition
            UpdateGenText(CurrentGenerator->GeneratorData.Quantity.Value, CurrentGenerator->GeneratorData.MaxTime.Value, CurrentGenerator->GeneratorData.Income.Value, CurrentGenerator->GeneratorData.GeneratorName);
        }
    }
    else
    {
        // Max time is already below or equal to the minimum threshold
        UpdateGenText(CurrentGenerator->GeneratorData.Quantity.Value, CurrentGenerator->GeneratorData.MaxTime.Value, CurrentGenerator->GeneratorData.Income.Value, CurrentGenerator->GeneratorData.GeneratorName);
        SpeedUpgradePriceText->SetText(FText::FromString("Max"));
    }
}



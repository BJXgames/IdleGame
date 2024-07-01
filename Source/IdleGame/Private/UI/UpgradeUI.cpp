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

	CurrentGenerator = MainGameInstance->CurrentSelectedGenerator;
}

void UUpgradeUI::UpdateGenText(FLargeNumber Quantity, FLargeNumber MaxTime, FLargeNumber Income, FString GenName)
{
	Income = Income * (1 / MaxTime.Value) * Quantity;
	Income.Normalize();

	Quantity.Normalize();
	MaxTime.Normalize();

	if(CurrentGenerator)
	{
		MainGameInstance->CurrentSelectedGenerator->GeneratorData.SpeedPrice.Normalize();
	}
	else
	{
		return;
	}
	
	
	GenIncomeText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(Income)));
	GenQuantityText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(Quantity)));
	GenSpeedText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(MaxTime) + "s"));
	GenNameText->SetText(FText::FromString(GenName));
	SpeedUpgradePriceText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(CurrentGenerator->GeneratorData.SpeedPrice)));

	if (CurrentGenerator->GeneratorData.SpeedUpgradeCount >= MaxSpeedUpgrades)
	{
		SpeedUpgradePriceText->SetText(FText::FromString("Max Upgrades"));
	}
	else
	{
		SpeedUpgradePriceText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(CurrentGenerator->GeneratorData.SpeedPrice)));
	}
	

	// UE_LOG(LogTemp, Warning, TEXT("speed: %s"), *WorldSubsystem->FormatLargeNumber(MaxTime))
}

void UUpgradeUI::UpgradeSpeed()
{
    const float MaxTimeReduction = 0.1f; // Amount to reduce max time by
    const float SpeedPriceMultiplier = 50.0f; // Multiplier for speed price

    if (CurrentGenerator->GeneratorData.SpeedUpgradeCount < MaxSpeedUpgrades)
    {
    	if (MainGameInstance->Money >= CurrentGenerator->GeneratorData.SpeedPrice)
    	{
    		// Deduct the speed upgrade cost
    		MainGameInstance->Money -= CurrentGenerator->GeneratorData.SpeedPrice;

    		// Increase the speed upgrade price for the next upgrade
    		CurrentGenerator->GeneratorData.SpeedPrice *= SpeedPriceMultiplier;
    		SpeedUpgradePriceText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(CurrentGenerator->GeneratorData.SpeedPrice)));

    		// Reduce the max time for the generator
    		CurrentGenerator->GeneratorData.MaxTime.Value -= MaxTimeReduction * CurrentGenerator->GeneratorData.MaxTime.Value;

    		// Increment the speed upgrade count
    		CurrentGenerator->GeneratorData.SpeedUpgradeCount++;

    		float AdjustedMaxTime = CurrentGenerator->GeneratorData.ManagerData.ManagerImage ? CurrentGenerator->GeneratorData.MaxTime.Value / CurrentGenerator->GeneratorData.ManagerData.SpeedBoost : CurrentGenerator->GeneratorData.MaxTime.Value;
    		FLargeNumber AdjustedIncome = CurrentGenerator->GeneratorData.ManagerData.ManagerImage ? CurrentGenerator->GeneratorData.Income * CurrentGenerator->GeneratorData.ManagerData.IncomeMultiplier : CurrentGenerator->GeneratorData.Income;
    		if(CurrentGenerator->GeneratorData.ManagerData.ManagerImage)
    		{
    			// Update the generator text if manager is equipped
    			UpdateGenText(CurrentGenerator->GeneratorData.Quantity, AdjustedMaxTime, AdjustedIncome, CurrentGenerator->GeneratorData.GeneratorName);
    		}
            else
            {
            	// Update the generator text if NO manager is equipped
            	UpdateGenText(CurrentGenerator->GeneratorData.Quantity, AdjustedMaxTime, AdjustedIncome, CurrentGenerator->GeneratorData.GeneratorName);
            }

    		
    	}
    }
    else
    {
        // Reached the maximum number of speed upgrades
        SpeedUpgradePriceText->SetText(FText::FromString("Max Upgrades"));
    }
}

UManagerUI* UUpgradeUI::GetManagerUI() const
{
	return BP_ManagerWidget;
}

void UUpgradeUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentGenerator = MainGameInstance->CurrentSelectedGenerator;
}



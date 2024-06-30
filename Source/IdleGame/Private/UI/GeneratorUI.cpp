// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GeneratorUI.h"

#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "MainWorldSubsystem.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/ManagerPanelUI.h"
#include "UI/UpgradeUI.h"

void UGeneratorUI::Buy()
{
	UpgradeUIWidget->CurrentGenerator = this;
	MainGameInstance->bIsBought = true;

	FLargeNumber i = FLargeNumber(0.0, 0);
	while (i < BuyMultiplier)
	{
		FLargeNumber AdjustedMoneyCost = GeneratorData.ManagerData.ManagerImage ? GeneratorData.MoneyCost * GeneratorData.ManagerData.MoneyPriceReduction : GeneratorData.MoneyCost;
		if (Product && AdjustedMoneyCost <= MainGameInstance->Money && GeneratorData.ProductCost <= Product->GeneratorData.Quantity)
		{
			MainGameInstance->Money -= AdjustedMoneyCost;
			Product->GeneratorData.Quantity -= GeneratorData.ProductCost;
			GeneratorData.Quantity += FLargeNumber(1.0, 0);
			GeneratorData.MoneyCost *= GeneratorCostMultiplier.Value;
			GeneratorData.MoneyCost.Normalize();

			++GeneratorData.GeneratorBought;

			if (GeneratorData.Quantity.Value == 1)
			{
				Time = 0;
			}

			CheckIfBuyAmountHasBeenReached();
			UpdateBuyButtonState();
			GenerateIncome();
		}
		else if (!Product && AdjustedMoneyCost <= MainGameInstance->Money)
		{
			MainGameInstance->Money -= AdjustedMoneyCost;
			GeneratorData.Quantity += FLargeNumber(1.0, 0);
			GeneratorData.MoneyCost *= GeneratorCostMultiplier.Value;
			GeneratorData.MoneyCost.Normalize();

			++GeneratorData.GeneratorBought;

			if (GeneratorData.Quantity.Value == 1)
			{
				Time = 0;
			}

			CheckIfBuyAmountHasBeenReached();
			UpdateBuyButtonState();
			GenerateIncome();
		}
		else
		{
			break;
		}
		++i;
	}

	CheckIfBuyAmountHasBeenReached();
	UpgradeUIWidget->UpdateGenText(GeneratorData.Quantity, GeneratorData.MaxTime.Value, GeneratorData.Income, GeneratorData.GeneratorName);
}


void UGeneratorUI::UpdateBuyButtonState()
{
	FLargeNumber AdjustedMoneyCost = GeneratorData.ManagerData.ManagerImage ? GeneratorData.MoneyCost * GeneratorData.ManagerData.MoneyPriceReduction : GeneratorData.MoneyCost;
	if (Product)
	{
		// Disable buy button if not enough product or money
		BuyButton->SetIsEnabled(Product->GeneratorData.Quantity >= GeneratorData.ProductCost && MainGameInstance->Money >= AdjustedMoneyCost);
	}
	else
	{
		// Disable buy button if not enough money
		BuyButton->SetIsEnabled(MainGameInstance->Money >= AdjustedMoneyCost * BuyMultiplier);
	}
}

void UGeneratorUI::GenerateIncome()
{
	if (GeneratorData.Quantity.Value > 0)
	{
		float AdjustedMaxTime = GeneratorData.MaxTime.Value;
		float AdjustedIncomeMultiplier = 1.0f;

		if (GeneratorData.ManagerData.ManagerImage)
		{
			AdjustedMaxTime /= GeneratorData.ManagerData.SpeedBoost;
			AdjustedIncomeMultiplier = GeneratorData.ManagerData.IncomeMultiplier;
		}

		if (Time >= AdjustedMaxTime || AdjustedMaxTime < 0.1f)
		{
			FLargeNumber IncomeToGenerate = GeneratorData.Income * GeneratorData.Quantity * AdjustedIncomeMultiplier;

			if (Product)
			{
				Product->GeneratorData.Quantity += IncomeToGenerate;
			}
			else
			{
				MainGameInstance->Money += IncomeToGenerate;
			}

			Time = (AdjustedMaxTime < 0.1f) ? 0 : (Time - AdjustedMaxTime); // Reset Time to 0 if below threshold
			UpdateProgressBar();
		}
	}
}

void UGeneratorUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Time += InDeltaTime;

	if (PlayerController)
	{
		if (!UpgradeUIWidget)
		{
			UpgradeUIWidget = PlayerController->GetUpgradeUI();
		}
	}

	if(GeneratorData.Quantity.Value > 0)
	{
		FLinearColor Color = FLinearColor::White;
		GeneratorBackground->SetBrushColor(Color);
		GeneratorBackground->SetIsEnabled(true);
		GeneratorButton->SetIsEnabled(true);
	}

	UpdateUIDisplays();
	UpdateProgressBar();
	CheckIncomeGeneration();
	UpdateBuyButtonState();
}

void UGeneratorUI::NativeConstruct()
{
	Super::NativeConstruct();
	WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();
	MainGameInstance = GetGameInstance<UMainGameInstance>();
	PlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();


	ProgressBar->SetPercent(0);
	UpdateUIDisplays();
	
}

void UGeneratorUI::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UGeneratorUI::UpdateUIDisplays()
{
	FLargeNumber AdjustedIncome = GeneratorData.ManagerData.ManagerImage ? GeneratorData.Income * GeneratorData.ManagerData.IncomeMultiplier : GeneratorData.Income;
	

	QuantityDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.Quantity)));
	IncomeDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(AdjustedIncome)));
	MoneyCostDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(MoneyCost)));
	ProductCostDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(ProductCost)));
	NameDisplay->SetText(FText::FromString(GeneratorData.GeneratorName));
}

void UGeneratorUI::CheckIncomeGeneration()
{
	if (GeneratorData.Quantity > FLargeNumber(0.0, 0) && Time >= GeneratorData.MaxTime.Value / GeneratorData.ManagerData.SpeedBoost)
	{
		GenerateIncome();
	}
}

void UGeneratorUI::UpdateProgressBar()
{
	const float MinMaxTime = 0.1f;

	if (GeneratorData.MaxTime.Value <= MinMaxTime)
	{
		// If the generator's speed is too high (MaxTime too low), show a full progress bar
		ProgressBar->SetPercent(1.0f);
	}
	else
	{
		float adjustedMaxTime = GeneratorData.MaxTime.Value;
		if (GeneratorData.Quantity > FLargeNumber(0.0, 0))
		{
			if (GeneratorData.ManagerData.ManagerImage)
			{
				adjustedMaxTime /= GeneratorData.ManagerData.SpeedBoost;
			}
			ProgressBar->SetPercent(FMath::Min(Time / adjustedMaxTime, 1.0f));
		}
	}
}

void UGeneratorUI::CheckIfBuyAmountHasBeenReached()
{
	AmountBoughtOfTheGeneratorDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.GeneratorBought)));
	AmountToReachIfBuyingGeneratorDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.AmountOfGeneratorToBuy)));
	
	if(GeneratorData.GeneratorBought >= GeneratorData.AmountOfGeneratorToBuy)
	{
		GeneratorData.AmountOfGeneratorToBuy *= 10;
		GeneratorData.Income *= 2;
	}
}

void UGeneratorUI::ToggleUpgradeWidget(UWidgetAnimation* Animation)
{
    if (UpgradeUIWidget)
    {
        // Set the current generator for the upgrade UI
        UpgradeUIWidget->CurrentGenerator = this;

        if (UpgradeUIWidget->IsVisible())
        {
            if (GeneratorData.GeneratorName == MainGameInstance->CurrentSelectedGenerator)
            {
                // If the same generator is clicked again, play the reverse animation to hide
                bIsHiding = true;
                UpgradeUIWidget->PlayAnimationReverse(Animation);
            	
                // Set visibility to collapsed after the animation finishes
                FTimerHandle TimerHandle;
                GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
                    UpgradeUIWidget->SetVisibility(ESlateVisibility::Collapsed);
                    bIsHiding = false;
                }, Animation->GetEndTime(), false);
            }
            else
            {
                // Update the text with the new generator details
                float AdjustedMaxTime = GeneratorData.ManagerData.ManagerImage ? GeneratorData.MaxTime.Value / GeneratorData.ManagerData.SpeedBoost : GeneratorData.MaxTime.Value;
                FLargeNumber AdjustedIncome = GeneratorData.ManagerData.ManagerImage ? GeneratorData.Income * GeneratorData.ManagerData.IncomeMultiplier : GeneratorData.Income;
                UpgradeUIWidget->UpdateGenText(GeneratorData.Quantity, AdjustedMaxTime, AdjustedIncome, GeneratorData.GeneratorName);
            }
        }
        else
        {
            // If the widget is not visible, set it to visible and play the animation
            bIsHiding = false;
        	
            float AdjustedMaxTime = GeneratorData.ManagerData.ManagerImage ? GeneratorData.MaxTime.Value / GeneratorData.ManagerData.SpeedBoost : GeneratorData.MaxTime.Value;
            FLargeNumber AdjustedIncome = GeneratorData.ManagerData.ManagerImage ? GeneratorData.Income * GeneratorData.ManagerData.IncomeMultiplier : GeneratorData.Income;
        	
            UpgradeUIWidget->UpdateGenText(GeneratorData.Quantity, AdjustedMaxTime, AdjustedIncome, GeneratorData.GeneratorName);
            UpgradeUIWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
            UpgradeUIWidget->PlayAnimation(Animation);
        }

        // Update the current selected generator in the game instance
        MainGameInstance->CurrentSelectedGenerator = GeneratorData.GeneratorName;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UpgradeUIWidget is null in UGeneratorUI::ToggleUpgradeWidget"));
    }
}

void UGeneratorUI::SelectGenerator()
{
	if (PlayerController)
	{
		UManagerPanelUI* ManagerPanel = PlayerController->GetManagerPanelUI();
		if (ManagerPanel)
		{
			ManagerPanel->UpdateGeneratorManager(this); // Updates the manager info
			ManagerPanel->UpdateManagerInfo(GeneratorData.ManagerData); // Display current manager info
		}
	}
}

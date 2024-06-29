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
#include "UI/ManagersInScrollBoxUI.h"
#include "UI/UpgradeUI.h"

void UGeneratorUI::Buy()
{
	UpgradeUIWidget->CurrentGenerator = this;
	MainGameInstance->bIsBought = true;

	FLargeNumber i = FLargeNumber(0.0, 0);
	while (i < BuyMultiplier)
	{
		if (Product && GeneratorData.MoneyCost <= MainGameInstance->Money && GeneratorData.ProductCost <= Product->GeneratorData.Quantity)
		{
			MainGameInstance->Money -= GeneratorData.MoneyCost;
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
		else if (!Product && GeneratorData.MoneyCost <= MainGameInstance->Money)
		{
			MainGameInstance->Money -= GeneratorData.MoneyCost;
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
	if (BuyMultiplier != FLargeNumber(1.0, 0))
	{
		if (Product)
		{
			// Disable buy button if not enough product or money
			BuyButton->SetIsEnabled(Product->GeneratorData.Quantity >= GeneratorData.ProductCost && MainGameInstance->Money >= MoneyCost);
		}
		else
		{
			// Disable buy button if not enough money
			BuyButton->SetIsEnabled(MainGameInstance->Money >= MoneyCost);
		}
	}
	else
	{
		if (Product)
		{
			// Disable buy button if not enough product or money
			BuyButton->SetIsEnabled(Product->GeneratorData.Quantity >= GeneratorData.ProductCost && MainGameInstance->Money >= GeneratorData.MoneyCost);
		}
		else
		{
			// Disable buy button if not enough money
			BuyButton->SetIsEnabled(MainGameInstance->Money >= GeneratorData.MoneyCost * BuyMultiplier);
		}
	}
}



void UGeneratorUI::GenerateIncome()
{
	if (GeneratorData.Quantity.Value > 0 && Time >= GeneratorData.MaxTime.Value)
	{
		if (Product)
		{
			Product->GeneratorData.Quantity += GeneratorData.Income * GeneratorData.Quantity;
		}
		else
		{
			MainGameInstance->Money += GeneratorData.Income * GeneratorData.Quantity;
		}
		Time -= GeneratorData.MaxTime.Value;
		UpdateProgressBar();
	}
}

void UGeneratorUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Time += InDeltaTime;

	UpdateUIDisplays();
	UpdateProgressBar();
	CheckIncomeGeneration();
	UpdateBuyButtonState();

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
	if (BuyMultiplier != FLargeNumber(1.0, 0))
	{
		QuantityDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.Quantity)));
		IncomeDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.Income)));
		MoneyCostDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(MoneyCost)));
		ProductCostDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(ProductCost)));
		NameDisplay->SetText(FText::FromString(GeneratorData.GeneratorName));
	}
	else
	{
		QuantityDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.Quantity)));
		IncomeDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.Income)));
		MoneyCostDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.MoneyCost)));
		ProductCostDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.ProductCost)));
		NameDisplay->SetText(FText::FromString(GeneratorData.GeneratorName));
	}
}

void UGeneratorUI::CheckIncomeGeneration()
{
	if (GeneratorData.Quantity > FLargeNumber(0.0, 0) && Time >= GeneratorData.MaxTime.Value)
	{
		GenerateIncome();
	}
}

void UGeneratorUI::UpdateProgressBar()
{
	if (GeneratorData.MaxTime > FLargeNumber(0.0, 0) && GeneratorData.Quantity > FLargeNumber(0.0, 0))
	{
		ProgressBar->SetPercent(Time / GeneratorData.MaxTime.Value);
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
                UpgradeUIWidget->UpdateGenText(GeneratorData.Quantity, GeneratorData.MaxTime.Value, GeneratorData.Income, GeneratorData.GeneratorName);
            }
        }
        else
        {
            // If the widget is not visible, set it to visible and play the animation
            bIsHiding = false;
            UpgradeUIWidget->UpdateGenText(GeneratorData.Quantity, GeneratorData.MaxTime.Value, GeneratorData.Income, GeneratorData.GeneratorName);
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
			ManagerPanel->UpdateManagerInfo(GeneratorData.ManagerData.Name,
				GeneratorData.ManagerData.SpeedBoost,
				GeneratorData.ManagerData.IncomeMultiplier,
				GeneratorData.ManagerData.MoneyPriceReduction,
				GeneratorData.ManagerData.ManagerImage); // Display current manager info
		}
	}
}

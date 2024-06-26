// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GeneratorUI.h"

#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "MainWorldSubsystem.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/UpgradeUI.h"

void UGeneratorUI::Buy()
{
	UpgradeUIWidget->CurrentGenerator = this;
	MainGameInstance->bIsBought = true;

	int i = 0;
	while (i < BuyMultiplier)
	{
		if (Product && GeneratorData.MoneyCost <= MainGameInstance->Money && GeneratorData.ProductCost <= Product->
			GeneratorData.Quantity)
		{
			MainGameInstance->Money -= GeneratorData.MoneyCost;
			Product->GeneratorData.Quantity -= GeneratorData.ProductCost;
			GeneratorData.Quantity += 1;
			GeneratorData.MoneyCost *= GeneratorCostMultiplier;

			if (GeneratorData.Quantity == 1)
			{
				Time = 0;
			}

			UpdateBuyButtonState();
			GenerateIncome();
		}
		else
		{
			MainGameInstance->Money -= GeneratorData.MoneyCost;
			GeneratorData.Quantity += 1;
			GeneratorData.MoneyCost *= GeneratorCostMultiplier;
		}
		i++;
	}

	UpgradeUIWidget->UpdateGenText(GeneratorData.Quantity, GeneratorData.MaxTime, GeneratorData.Income,
	                               GeneratorData.GeneratorName);
}

void UGeneratorUI::UpdateBuyButtonState()
{
	if (BuyMultiplier != 1)
	{
		if (Product)
		{
			// Disable buy button if not enough product or money
			BuyButton->SetIsEnabled(ProductCost <= Product->GeneratorData.Quantity && MoneyCost <= MainGameInstance->Money);
		}
		else
		{
			// Disable buy button if not enough money
			BuyButton->SetIsEnabled(MoneyCost <= MainGameInstance->Money);
		}
	}
	else
	{
		if (Product)
		{
			// Disable buy button if not enough product or money
			BuyButton->SetIsEnabled(
				ProductCost <= Product->GeneratorData.Quantity && GeneratorData.MoneyCost <= MainGameInstance->Money);
		}
		else
		{
			// Disable buy button if not enough money
			BuyButton->SetIsEnabled(GeneratorData.MoneyCost * BuyMultiplier <= MainGameInstance->Money);
		}
	}
}


void UGeneratorUI::GenerateIncome()
{
	if (GeneratorData.Quantity > 0 && Time >= GeneratorData.MaxTime)
	{
		if (Product)
		{
			Product->GeneratorData.Quantity += GeneratorData.Income * GeneratorData.Quantity;
		}
		else
		{
			MainGameInstance->Money += GeneratorData.Income * GeneratorData.Quantity;
		}
		Time -= GeneratorData.MaxTime;
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
			UpgradeUIWidget = PlayerController->UpgradeUI;
		}
		if (!UpgradeUIWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("UpgradeUIWidget is null in UGeneratorUI::NativeConstruct"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null in UGeneratorUI::NativeConstruct"));
	}

	if(GeneratorData.Quantity > 0)
	{
		FLinearColor Color = FLinearColor::White;
		GeneratorBackground->SetBrushColor(Color);
		SetIsEnabled(true);
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
	if (BuyMultiplier != 1)
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
	if (GeneratorData.Quantity > 0 && Time >= GeneratorData.MaxTime)
	{
		GenerateIncome();
	}
}

void UGeneratorUI::UpdateProgressBar()
{
	if (GeneratorData.MaxTime > 0 && GeneratorData.Quantity > 0)
	{
		ProgressBar->SetPercent(Time / GeneratorData.MaxTime);
	}
}

void UGeneratorUI::ToggleUpgradeWidget(UWidgetAnimation* Animation)
{
	if (UpgradeUIWidget)
	{
		UpgradeUIWidget->CurrentGenerator = this;
		if (PlayerController->UpgradeUI->IsVisible())
		{
			if (GeneratorData.GeneratorName == MainGameInstance->CurrentSelectedGenerator)
			{
				bIsHiding = true;
				UpgradeUIWidget->PlayAnimationReverse(Animation);
			}
			else
			{
				UpgradeUIWidget->UpdateGenText(GeneratorData.Quantity, GeneratorData.MaxTime, GeneratorData.Income,
											   GeneratorData.GeneratorName);
			}
		}
		else
		{
			bIsHiding = false;
			UpgradeUIWidget->UpdateGenText(GeneratorData.Quantity, GeneratorData.MaxTime, GeneratorData.Income,
										   GeneratorData.GeneratorName);
			UpgradeUIWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			UpgradeUIWidget->PlayAnimation(Animation);
		}
		MainGameInstance->CurrentSelectedGenerator = GeneratorData.GeneratorName;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UpgradeUIWidget is null in UGeneratorUI::ToggleUpgradeWidget"));
	}
	
}

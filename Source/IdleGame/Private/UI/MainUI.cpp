// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUI.h"
#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "MainWorldSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ManagerPanelUI.h"
#include "UI/UpgradeUI.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	MainGameInstance = GetGameInstance<UMainGameInstance>();
	PlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
	WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();
	AddGeneratorToTheScrollBox();
}

void UMainUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	

}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentMoneyText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(MainGameInstance->Money)));

	UpdateBuyMultiplier();
	UpdateIncomePerSecond();
}

void UMainUI::UpdateBuyMultiplier()
{
	if(bIsbuyx1Active)
	{
		buyx1();
	}
	if (bIsbuyMaxActive)
	{
		buyxMax();
	}
	else if (bIsbuyx5Active)
	{
		buyx5();
	}
	else if (bIsbuyx10Active)
	{
		buyx10();
	}
}

void UMainUI::UpdateGeneratorCostsAndUI(UGeneratorUI* Generator, FLargeNumber Multiplier)
{
	// Apply manager price reduction if applicable
	FLargeNumber AdjustedMoneyCost = Generator->GeneratorData.ManagerData.ManagerImage ?
									 Generator->GeneratorData.MoneyCost * Generator->GeneratorData.ManagerData.MoneyPriceReduction : 
									 Generator->GeneratorData.MoneyCost;

	Generator->MoneyCost = AdjustedMoneyCost;
	Generator->BuyMultiplier = Multiplier;
	Generator->ProductCost = Generator->GeneratorData.ProductCost;

	FLargeNumber TempValue = AdjustedMoneyCost;
	FLargeNumber TempProductCost = Generator->GeneratorData.ProductCost;

	for (FLargeNumber i = FLargeNumber(1, 0); i < Multiplier; ++i)
	{
		TempValue *= Generator->GeneratorCostMultiplier;
		Generator->MoneyCost += TempValue;
		Generator->ProductCost += TempProductCost;
	}

	Generator->AmountAddedToQuantityDisplay->SetText(FText::FromString("+" + WorldSubsystem->FormatLargeNumber(Generator->BuyMultiplier)));
}


void UMainUI::buyx1()
{
	bIsbuyx1Active = true;
	bIsbuyx5Active = false;
	bIsbuyx10Active = false;
	bIsbuyMaxActive = false;

	for (UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		UpdateGeneratorCostsAndUI(Generator, FLargeNumber(1.0, 0));
	}
}

void UMainUI::buyx5()
{
	bIsbuyx1Active = false;
	bIsbuyx10Active = false;
	bIsbuyMaxActive = false;

	if (bIsbuyx5Active && !MainGameInstance->bIsBought)
	{
		return;
	}

	bIsbuyx5Active = true;
	for (UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		UpdateGeneratorCostsAndUI(Generator, FLargeNumber(5.0, 0));
	}
	MainGameInstance->bIsBought = false;
}

void UMainUI::buyx10()
{
	bIsbuyx1Active = false;
	bIsbuyx5Active = false;
	bIsbuyMaxActive = false;

	if (bIsbuyx10Active && !MainGameInstance->bIsBought)
	{
		return;
	}

	bIsbuyx10Active = true;
	for (UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		UpdateGeneratorCostsAndUI(Generator, FLargeNumber(10.0, 0));
	}
	MainGameInstance->bIsBought = false;
}

void UMainUI::buyxMax()
{
	bIsbuyx1Active = false;
	bIsbuyx5Active = false;
	bIsbuyx10Active = false;
	bIsbuyMaxActive = true;

	for (UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		FLargeNumber availableMoney = MainGameInstance->Money;
		FLargeNumber currentMoneyCost = Generator->GeneratorData.MoneyCost;
		FLargeNumber currentProductCost = Generator->GeneratorData.ProductCost;
		FLargeNumber buyCount = FLargeNumber(0.0, 0);

		// Adjust the money cost considering the manager's price reduction
		if (Generator->GeneratorData.ManagerData.ManagerImage)
		{
			currentMoneyCost *= Generator->GeneratorData.ManagerData.MoneyPriceReduction;
		}

		if (Generator->Product)
		{
			FLargeNumber availableProductQuantity = Generator->Product->GeneratorData.Quantity;

			while (availableMoney >= currentMoneyCost && availableProductQuantity >= currentProductCost)
			{
				availableMoney -= currentMoneyCost;
				availableProductQuantity -= currentProductCost;
				++buyCount;
				currentMoneyCost *= Generator->GeneratorCostMultiplier;
				currentProductCost += Generator->GeneratorData.ProductCost;
			}
		}
		else
		{
			while (availableMoney >= currentMoneyCost)
			{
				availableMoney -= currentMoneyCost;
				++buyCount;
				currentMoneyCost *= Generator->GeneratorCostMultiplier;
			}
		}
		UpdateGeneratorCostsAndUI(Generator, buyCount);
	}

	MainGameInstance->bIsBought = false;
}

void UMainUI::DeleteSave()
{
	// Delete the save file in the specified slot
	UGameplayStatics::DeleteGameInSlot("Slot1", 0);
	MainGameInstance->CreateSaveFile();
	AddGeneratorToTheScrollBox();
	UpdateIncomePerSecond();
	PlayerController->GetUpgradeUI()->SetVisibility(ESlateVisibility::Collapsed);
	buyx1();

	MainGameInstance->ManagersUnlockedDataTable->EmptyTable();
	PlayerController->GetManagerPanelUI()->UpdateManagersToGrid();
	PlayerController->GetManagerPanelUI()->ResetManagersPanelText();
}

void UMainUI::AddTime()
{
	for (UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		if (Generator->GeneratorData.Quantity.Value > 0)
		{
			// Calculate the income generated during the offline time and add it immediately
			FLargeNumber TimesTriggered = FLargeNumber(100000.0f / Generator->GeneratorData.MaxTime.Value, 0);
			FLargeNumber IncomeGenerated = Generator->GeneratorData.Income * Generator->GeneratorData.Quantity * TimesTriggered;

			if (Generator->Product)
			{
				Generator->Product->GeneratorData.Quantity += IncomeGenerated;
			}
			else
			{
				MainGameInstance->Money += IncomeGenerated;
			}
		}
	}
	
	MainGameInstance->AddManagerToInventory();
	//PlayerController->GetManagerPanelUI()->UpdateManagersToGrid();
}


void UMainUI::AddGeneratorToTheScrollBox()
{
	/*ScrollBox->ClearChildren();
	for (UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		if (IsValid(Generator))
		{
			// Set padding for the generator
			FMargin NewPadding = Generator->GetPadding();
			NewPadding.Top = 10.0f; // Adjust the top padding as needed
			Generator->SetPadding(NewPadding);

			// Add the generator to the scroll box
			ScrollBox->AddChild(Generator);
		}
	}*/

	// Clear the existing children of the scroll box
	UniformGrid->ClearChildren();

	for (int32 i = 0; i < 5; i++)
	{
		if (IsValid(MainGameInstance->Generators[i]))
		{
			UniformGrid->AddChildToUniformGrid(MainGameInstance->Generators[i], i, 0);
		}
	}
	
	for (int32 i = 5; i < 10; i++)
	{
		if (IsValid(MainGameInstance->Generators[i]))
		{
			UniformGrid->AddChildToUniformGrid(MainGameInstance->Generators[i], i - 5, 1);
		}
	}
}

void UMainUI::UpdateIncomePerSecond()
{
	IncomePerSecond = FLargeNumber(0.0, 0);
	float AdjustedMaxTime = MainGameInstance->Generators[0]->GeneratorData.MaxTime.Value;
	FLargeNumber AdjustedIncome = MainGameInstance->Generators[0]->GeneratorData.Income;
	if (MainGameInstance->Generators[0]->GeneratorData.ManagerData.ManagerImage)
	{
		AdjustedMaxTime /= MainGameInstance->Generators[0]->GeneratorData.ManagerData.SpeedBoost;
		AdjustedIncome *= MainGameInstance->Generators[0]->GeneratorData.ManagerData.IncomeMultiplier;
	}
	FLargeNumber GeneratorIncomePerSecond = AdjustedIncome * (1 / AdjustedMaxTime) * MainGameInstance->Generators[0]->GeneratorData.Quantity;
	IncomePerSecond += GeneratorIncomePerSecond;
    
	// Debug log to check the values before formatting
	//UE_LOG(LogTemp, Warning, TEXT("IncomePerSecond: Value = %f, MagnitudeIndex = %d"), IncomePerSecond.Value, IncomePerSecond.MagnitudeIndex);
    
	// Update the text with formatted income per second
	FString FormattedIncomePerSecond = WorldSubsystem->FormatLargeNumber(IncomePerSecond) + " Per Second";
	IncomePerSecondText->SetText(FText::FromString(FormattedIncomePerSecond));
	
}



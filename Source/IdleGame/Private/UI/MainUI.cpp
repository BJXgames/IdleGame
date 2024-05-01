// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUI.h"

#include "MainGameInstance.h"
#include "MainWorldSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"


void UMainUI::UpdateBuyMultiplier()
{
	if (bIsbuyMaxActive)
	{
		buyxMax();
	}
	if (bIsbuyx5Active)
	{
		buyx5();
	}
	if (bIsbuyx10Active)
	{
		buyx10();
	}
}

void UMainUI::buyx1()
{
	bIsbuyx1Active = true;
	bIsbuyx5Active = false;
	bIsbuyx10Active = false;
	bIsbuyMaxActive = false;

	for (UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		Generator->MoneyCost = Generator->GeneratorData.MoneyCost;
		Generator->BuyMultiplier = 1;
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
		Generator->MoneyCost = Generator->GeneratorData.MoneyCost;
		Generator->BuyMultiplier = 5;
		double TempValue = Generator->GeneratorData.MoneyCost;

		int i = 0;
		while (i < Generator->BuyMultiplier - 1)
		{
			TempValue = TempValue * Generator->GeneratorCostMultiplier;
			Generator->MoneyCost += TempValue;
			i++;
		}
		//UE_LOG(LogTemp, Warning, TEXT("Monecost: %s"), *WorldSubsystem->FormatLargeNumber(Generator->MoneyCost))
	}
	MainGameInstance->bIsBought = false;
}

void UMainUI::buyx10()
{
	bIsbuyx1Active = false;
	bIsbuyx5Active = false;
	bIsbuyMaxActive = false;

	if(bIsbuyx10Active && !MainGameInstance->bIsBought)
	{
		return;
	}

	bIsbuyx10Active = true;
	for (UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		Generator->MoneyCost = Generator->GeneratorData.MoneyCost;
		Generator->BuyMultiplier = 10;
		double TempValue = Generator->GeneratorData.MoneyCost;

		int i = 0;
		while (i < Generator->BuyMultiplier - 1)
		{
			TempValue = TempValue * Generator->GeneratorCostMultiplier;
			Generator->MoneyCost += TempValue;
			i++;
		}
	}
	MainGameInstance->bIsBought = false;
}

void UMainUI::buyxMax()
{
	bIsbuyx1Active = false;
	bIsbuyx5Active = false;
	bIsbuyx10Active = false;
	bIsbuyMaxActive = true;

	for (int32 i = 0; i < MainGameInstance->Generators.Num(); i++)
	{
		UGeneratorUI* CurrentGenerator = MainGameInstance->Generators[i];

		if (CurrentGenerator)
		{
			// Check if the generator is a product of another generator
			if (CurrentGenerator->Product)
			{
				// Check if the product cost is less than the quantity of the parent generator
				if (CurrentGenerator->GeneratorData.ProductCost < CurrentGenerator->Product->GeneratorData.Quantity)
				{
					if (FMath::Floor(MainGameInstance->Money / CurrentGenerator->GeneratorData.MoneyCost) >= 1)
					{
						CurrentGenerator->BuyMultiplier = FMath::Floor(FMath::Min(
							MainGameInstance->Money / CurrentGenerator->GeneratorData.MoneyCost,
							CurrentGenerator->Product->GeneratorData.Quantity / CurrentGenerator->GeneratorData.
							ProductCost));
					}
					else
					{
						CurrentGenerator->BuyMultiplier = 1;
					}
				}
				else
				{
					// Player can only afford one if the product cost is higher than the parent's quantity
					CurrentGenerator->BuyMultiplier = 1;
				}
			}
			else
			{
				// Calculate the maximum affordable quantity based on money
				CurrentGenerator->BuyMultiplier = FMath::Floor(
					MainGameInstance->Money / CurrentGenerator->GeneratorData.MoneyCost);
			}

			// Ensure the multiplier is not negative
			CurrentGenerator->BuyMultiplier = FMath::Max(CurrentGenerator->BuyMultiplier, 1);
		}
	}
}

void UMainUI::DeleteSave()
{
	// Delete the save file in the specified slot
	UGameplayStatics::DeleteGameInSlot("Slot1", 0);
	MainGameInstance->CreateSaveFile();
	AddGeneratorToTheScrollBox();
	buyx1();
}

void UMainUI::AddTime()
{
	for (UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		if (Generator->GeneratorData.Quantity > 0)
		{
			// Calculate the income generated during the offline time and add it immediately
			double TimesTriggered = 1000 / Generator->GeneratorData.MaxTime;
			if (Generator->Product)
			{
				Generator->Product->GeneratorData.Quantity += Generator->GeneratorData.Income * Generator->GeneratorData
					.Quantity * TimesTriggered;
			}
			else
			{
				MainGameInstance->Money += Generator->GeneratorData.Income * Generator->GeneratorData.Quantity *
					TimesTriggered;
			}
		}
	}
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
	IncomePerSecond = 0;
	IncomePerSecond += MainGameInstance->Generators[0]->GeneratorData.Income * (1 / MainGameInstance->Generators[0]->
		GeneratorData.MaxTime) * MainGameInstance->Generators[0]->GeneratorData.Quantity;

	IncomePerSecondText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(IncomePerSecond) + " Per Second"));
}


void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
	MainGameInstance = GetGameInstance<UMainGameInstance>();
	WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();
	AddGeneratorToTheScrollBox();
	buyx1();
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

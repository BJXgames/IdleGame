// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUI.h"

#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "MainWorldSubsystem.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UpgradeUI.h"


void UMainUI::UpdateBuyMultiplier()
{
	if(bIsbuyMaxActive)
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
	
	for(UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		Generator->MoneyCost = Generator->GeneratorData.MoneyCost;
		Generator->BuyMultiplier = 1;
		Generator->ProductCost = Generator->GeneratorData.ProductCost;
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
		double TempProductCost = Generator->GeneratorData.ProductCost;
		Generator->ProductCost = TempProductCost;

		int i = 0;
		while (i < Generator->BuyMultiplier - 1)
		{
			TempValue *= Generator->GeneratorCostMultiplier;
			Generator->MoneyCost += TempValue;
			Generator->ProductCost += TempProductCost;
			i++;
		}
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
		Generator->MoneyCost = Generator->GeneratorData.MoneyCost;
		Generator->BuyMultiplier = 10;
		double TempValue = Generator->GeneratorData.MoneyCost;
		double TempProductCost = Generator->GeneratorData.ProductCost;
		Generator->ProductCost = TempProductCost;

		int i = 0;
		while (i < Generator->BuyMultiplier - 1)
		{
			TempValue *= Generator->GeneratorCostMultiplier;
			Generator->MoneyCost += TempValue;
			Generator->ProductCost += TempProductCost;
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

    for (UGeneratorUI* Generator : MainGameInstance->Generators)
    {
        Generator->MoneyCost = Generator->GeneratorData.MoneyCost;

        // Calculate how many times you can buy the generator
        double availableMoney = MainGameInstance->Money;
        double currentMoneyCost = Generator->GeneratorData.MoneyCost;
        double currentProductCost = Generator->GeneratorData.ProductCost;
    	Generator->ProductCost = currentProductCost;
        int buyCount = 0;

    	if(Generator->Product)
    	{
    		double availableProductQuantity = Generator->Product->GeneratorData.Quantity;
    		
    		while (availableMoney >= currentMoneyCost && availableProductQuantity >= currentProductCost)
    		{
    			availableMoney -= currentMoneyCost;
    			availableProductQuantity -= currentProductCost;
    			buyCount++;
    			currentMoneyCost *= Generator->GeneratorCostMultiplier;
    			currentProductCost += Generator->GeneratorData.ProductCost;
    		}
    	}
        else
        {
        	while (availableMoney >= currentMoneyCost)
        	{
        		availableMoney -= currentMoneyCost;
        		buyCount++;
        		currentMoneyCost *= Generator->GeneratorCostMultiplier;
        	}
        }
        

        Generator->BuyMultiplier = buyCount;

        // If we can buy at least one, update the MoneyCost and ProductCost to reflect the cumulative cost of buyCount purchases
        if (buyCount > 0)
        {
            Generator->MoneyCost = Generator->GeneratorData.MoneyCost;
            Generator->ProductCost = Generator->GeneratorData.ProductCost;
            double tempMoneyValue = Generator->GeneratorData.MoneyCost;
            double tempProductValue = Generator->GeneratorData.ProductCost;

            for (int i = 1; i < buyCount; i++)
            {
                tempMoneyValue *= Generator->GeneratorCostMultiplier;
                Generator->MoneyCost += tempMoneyValue;
                Generator->ProductCost += tempProductValue;
            }
        }
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
	PlayerController->UpgradeUI->SetVisibility(ESlateVisibility::Collapsed);
	buyx1();
	
}

void UMainUI::AddTime()
{
	for(UGeneratorUI* Generator : MainGameInstance->Generators)
	{
		if (Generator->GeneratorData.Quantity > 0)
		{
			// Calculate the income generated during the offline time and add it immediately
			double TimesTriggered = 1000 / Generator->GeneratorData.MaxTime;
			if (Generator->Product)
			{
				Generator->Product->GeneratorData.Quantity += Generator->GeneratorData.Income * Generator->GeneratorData.Quantity * TimesTriggered;
			}
			else
			{
				MainGameInstance->Money += Generator->GeneratorData.Income * Generator->GeneratorData.Quantity * TimesTriggered;
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
	IncomePerSecond += MainGameInstance->Generators[0]->GeneratorData.Income * (1 / MainGameInstance->Generators[0]->GeneratorData.MaxTime) * MainGameInstance->Generators[0]->GeneratorData.Quantity;
	
	IncomePerSecondText->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(IncomePerSecond) + " Per Second"));
}


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

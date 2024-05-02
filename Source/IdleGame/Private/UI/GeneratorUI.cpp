// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GeneratorUI.h"

#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "MainWorldSubsystem.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/UpgradeUI.h"

void UGeneratorUI::Buy()
{
    PlayerController->UpgradeUI->CurrentGenerator = this;
    MainGameInstance->bIsBought = true;
    
    int i = 0;
    while (i < BuyMultiplier)
    {
        if (Product && GeneratorData.MoneyCost <= MainGameInstance->Money && GeneratorData.ProductCost <= Product->GeneratorData.Quantity)
        {
            MainGameInstance->Money -= GeneratorData.MoneyCost;
            Product->GeneratorData.Quantity -= GeneratorData.ProductCost;
            GeneratorData.Quantity += 1;
            GeneratorData.MoneyCost *= GeneratorCostMultiplier;
            
            if(GeneratorData.Quantity == 1)
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
    
    PlayerController->UpgradeUI->UpdateGenText(GeneratorData.Quantity, GeneratorData.MaxTime, GeneratorData.Income, GeneratorData.GeneratorName);
}

void UGeneratorUI::UpdateBuyButtonState()
{
    if (Product)
    {
        // Disable buy button if not enough product or money
        BuyButton->SetIsEnabled(GeneratorData.ProductCost * BuyMultiplier <= Product->GeneratorData.Quantity && GeneratorData.MoneyCost * BuyMultiplier <= MainGameInstance->Money);
    }
    else
    {
        // Disable buy button if not enough money
        BuyButton->SetIsEnabled(GeneratorData.MoneyCost * BuyMultiplier <= MainGameInstance->Money);
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
    QuantityDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.Quantity)));
    IncomeDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.Income)));
    MoneyCostDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.MoneyCost * BuyMultiplier)));
    ProductCostDisplay->SetText(FText::FromString(WorldSubsystem->FormatLargeNumber(GeneratorData.ProductCost * BuyMultiplier)));
    NameDisplay->SetText(FText::FromString(GeneratorData.GeneratorName));
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

void UGeneratorUI::OpenUpgradeWidget()
{
    //UE_LOG(LogTemp, Warning, TEXT("Selected Generator: %s"), *MainGameInstance->CurrentSelectedGenerator)
    //UE_LOG(LogTemp, Warning, TEXT("Clicked on Generator: %s"), *GeneratorData.GeneratorName)
    PlayerController->UpgradeUI->CurrentGenerator = this;
    if(PlayerController->UpgradeUI->IsVisible())
    {
        if(GeneratorData.GeneratorName == MainGameInstance->CurrentSelectedGenerator)
        {
            PlayerController->UpgradeUI->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            PlayerController->UpgradeUI->UpdateGenText(GeneratorData.Quantity, GeneratorData.MaxTime, GeneratorData.Income, GeneratorData.GeneratorName);
        }
    }
    else
    {
        PlayerController->UpgradeUI->UpdateGenText(GeneratorData.Quantity, GeneratorData.MaxTime, GeneratorData.Income, GeneratorData.GeneratorName);
        PlayerController->UpgradeUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    MainGameInstance->CurrentSelectedGenerator = GeneratorData.GeneratorName;
    
}



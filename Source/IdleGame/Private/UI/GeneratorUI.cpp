// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GeneratorUI.h"

#include "MainGameInstance.h"
#include "MainWorldSubsystem.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UGeneratorUI::Buy()
{
    
    if (Product && BuyMultiplier * GeneratorData.MoneyCost <= MainGameInstance->Money && BuyMultiplier * GeneratorData.ProductCost <= Product->GeneratorData.Quantity)
    {
        MainGameInstance->Money -= BuyMultiplier * GeneratorData.MoneyCost;
        Product->GeneratorData.Quantity -= BuyMultiplier * GeneratorData.ProductCost;
        GeneratorData.Quantity += BuyMultiplier;
        
        if(GeneratorData.Quantity == BuyMultiplier)
        {
            Time = 0;
        }
        
        UpdateBuyButtonState();
        GenerateIncome();
    }
    else
    {
        MainGameInstance->Money -= GeneratorData.MoneyCost * BuyMultiplier;
        GeneratorData.Quantity += BuyMultiplier;

        UpdateBuyButtonState();
        GenerateIncome();
    }
}

void UGeneratorUI::UpdateBuyButtonState()
{
    if (Product)
    {
        // Disable buy button if not enough product or money
        BuyButton->SetIsEnabled(GeneratorData.ProductCost * BuyMultiplier <= Product->GeneratorData.Quantity && GeneratorData.MoneyCost * BuyMultiplier <= MainGameInstance->Money);
        //UE_LOG(LogTemp, Warning, TEXT("Calculated ProductCost: %.0LF"), GeneratorData.ProductCost * MainGameInstance->SelectedMultiplier)
        //UE_LOG(LogTemp, Warning, TEXT("Calculated Product Quantity: %.0LF"), Product->GeneratorData.Quantity * MainGameInstance->SelectedMultiplier)
    }
    else
    {
        // Disable buy button if not enough money
        BuyButton->SetIsEnabled(GeneratorData.MoneyCost * BuyMultiplier <= MainGameInstance->Money);
        //UE_LOG(LogTemp, Warning, TEXT("Calculated cost: %.0LF"), GeneratorData.MoneyCost * MainGameInstance->SelectedMultiplier)
        //UE_LOG(LogTemp, Warning, TEXT("Calculated TotalMoney: %.0LF"), MainGameInstance->Money * MainGameInstance->SelectedMultiplier)
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
    ProgressBar->SetPercent(0);
    UpdateUIDisplays();
    
}

void UGeneratorUI::NativePreConstruct()
{
    Super::NativePreConstruct();
    WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();
    MainGameInstance = GetGameInstance<UMainGameInstance>();
    
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



// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "IdleGameSave.h"
#include "UI/GeneratorUI.h"
#include "MainWorldSubsystem.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"


float UMainGameInstance::CalculateOfflineTime()
{
    // Calculate offline time
    FString SaveFilePath = FPaths::ProjectSavedDir() + TEXT("SaveGames/Slot1.sav");
    DataToLoad->SaveTime = IFileManager::Get().GetTimeStamp(*SaveFilePath);
    
    FDateTime CurrentTime = FDateTime::UtcNow();
    float TimeElapsed = (CurrentTime - DataToLoad->SaveTime).GetTotalSeconds();
    float MaxOfflineTime = 43200; // Maximum offline time in seconds (12 hours)

    // Ensure the calculated offline time doesn't exceed the maximum allowed time
    TimeElapsed = FMath::Min(TimeElapsed, MaxOfflineTime);
    TimeElapsed = FMath::Max(TimeElapsed, 0.0); // Ensure time elapsed is not negative
    
    //UE_LOG(LogTemp, Warning, TEXT("TimeElapsed: %.0LF"), TimeElapsed)
    
    return TimeElapsed;
}



UMainGameInstance::UMainGameInstance()
{
    Money = FLargeNumber(1.0, 0);

    static ConstructorHelpers::FObjectFinder<UDataTable> ManagerDataTableObj(TEXT("/Game/Data/ManagerDataTable"));
    if (ManagerDataTableObj.Succeeded())
    {
        ManagerDataTable = ManagerDataTableObj.Object;
    }
}

void UMainGameInstance::InitGenerators()
{
    // Clear existing generators
    Generators.Empty();

    for (int32 i = 0; i < NumberOfGenerators; i++)
    {
        UGeneratorUI* NewGenerator = CreateWidget<UGeneratorUI>(GetWorld(), GeneratorUIClass);
        NewGenerator->GeneratorData.Income = FLargeNumber(1.0, 0);
        NewGenerator->GeneratorData.Quantity = FLargeNumber((i == 0) ? 1.0 : 0.0, 0);
        NewGenerator->GeneratorData.MoneyCost = FLargeNumber((i == 0) ? 10.0 : (i * i * i * i * i * 1000.0), 0);
        NewGenerator->GeneratorData.ProductCost = FLargeNumber((i == 0) ? 0.0 : i * (3 + i), 0);
        NewGenerator->GeneratorData.MaxTime = FLargeNumber((i == 0) ? 1 : i * 2.0, 0);
        NewGenerator->GeneratorData.GeneratorName = FString::Printf(TEXT("Gen %d"), i + 1);
        NewGenerator->GeneratorData.SpeedPrice = FLargeNumber(1.0, 0);
        NewGenerator->GeneratorData.AmountOfGeneratorToBuy = FLargeNumber(100.0, 0);

        if(i > 0)
        {
            NewGenerator->Product = Generators[i - 1];
            NewGenerator->GeneratorData.GeneratorBought = FLargeNumber(0.0, 0);
        }
        else
        {
            NewGenerator->GeneratorData.GeneratorBought = FLargeNumber(1.0, 0);
        }

        if(NewGenerator->GeneratorData.Quantity.Value <= 0)
        {
            FLinearColor CustomColor(.1, .1, .1, 1.0);
            NewGenerator->GeneratorBackground->SetBrushColor(CustomColor);
            NewGenerator->GeneratorBackground->SetIsEnabled(false);
            NewGenerator->GeneratorButton->SetIsEnabled(false);
        }

        NewGenerator->CheckIfBuyAmountHasBeenReached();

        //UE_LOG(LogTemp, Warning, TEXT("Loaded Generator %d: Name=%s, Quantity=%.0f"), i, *NewGenerator->GeneratorData.GeneratorName, NewGenerator->GeneratorData.Quantity);
        
        Generators.Add(NewGenerator);
    }
    
}



void UMainGameInstance::CreateSaveFile()
{
    DataToSave = Cast<UIdleGameSave>(UGameplayStatics::CreateSaveGameObject(UIdleGameSave::StaticClass()));
	UGameplayStatics::SaveGameToSlot(DataToSave, "Slot1", 0);
    UE_LOG(LogTemp, Warning, TEXT("New file created!"))
    Money = 0;
    InitGenerators();
}

void UMainGameInstance::SaveGame()
{
    DataToSave = Cast<UIdleGameSave>(UGameplayStatics::CreateSaveGameObject(UIdleGameSave::StaticClass()));

    if (DataToSave != nullptr)
    {
        DataToSave->Money = Money;

        // Clear arrays to avoid appending duplicate data if this function is called multiple times
        DataToSave->Gens.GeneratorNames.Empty();
        DataToSave->Gens.Quantities.Empty();
        DataToSave->Gens.Incomes.Empty();
        DataToSave->Gens.MoneyCosts.Empty();
        DataToSave->Gens.ProductCosts.Empty();
        DataToSave->Gens.MaxTimes.Empty();
        DataToSave->Gens.SpeedPrices.Empty();
        DataToSave->Gens.GeneratorsBought.Empty();
        DataToSave->Gens.AmountOfGeneratorsToBuy.Empty();
        
        for (int32 i = 0; i < Generators.Num(); i++)
        {
            UGeneratorUI* CurrentGenerator = Generators[i];
            if (IsValid(CurrentGenerator))
            {
                DataToSave->Gens.GeneratorNames.Add(CurrentGenerator->GeneratorData.GeneratorName);
                DataToSave->Gens.Quantities.Add(CurrentGenerator->GeneratorData.Quantity);
                DataToSave->Gens.Incomes.Add(CurrentGenerator->GeneratorData.Income);
                DataToSave->Gens.MoneyCosts.Add(CurrentGenerator->GeneratorData.MoneyCost);
                DataToSave->Gens.ProductCosts.Add(CurrentGenerator->GeneratorData.ProductCost);
                DataToSave->Gens.MaxTimes.Add(CurrentGenerator->GeneratorData.MaxTime);
                DataToSave->Gens.SpeedPrices.Add(CurrentGenerator->GeneratorData.SpeedPrice);
                DataToSave->Gens.GeneratorsBought.Add(CurrentGenerator->GeneratorData.GeneratorBought); 
                DataToSave->Gens.AmountOfGeneratorsToBuy.Add(CurrentGenerator->GeneratorData.AmountOfGeneratorToBuy); 

                //UE_LOG(LogTemp, Warning, TEXT(": %s, GeneratorsBought: %.0f"), *DataToSave->Gens.GeneratorNames[i], DataToSave->Gens.GeneratorsBought[i].Value);
            }
        }

        UGameplayStatics::SaveGameToSlot(DataToSave, "Slot1", 0);
    }
    else
    {
        CreateSaveFile();
        UE_LOG(LogTemp, Warning, TEXT("Save"))
    }
}



void UMainGameInstance::LoadGame()
{
    DataToLoad = Cast<UIdleGameSave>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

    if (DataToLoad != nullptr)
    {
        Money = DataToLoad->Money;
        StartMoney = Money;

        if (DataToLoad->Gens.GeneratorNames.Num() > 0)
        {
            // Load data for each generator
            for (int32 i = 0; i < NumberOfGenerators; i++)
            {
                UGeneratorUI* NewGenerator = CreateWidget<UGeneratorUI>(GetWorld(), GeneratorUIClass);
                if (NewGenerator)
                {
                    // Set the saved data to the new generator instance
                    NewGenerator->GeneratorData.GeneratorName = DataToLoad->Gens.GeneratorNames[i];
                    NewGenerator->GeneratorData.Quantity = DataToLoad->Gens.Quantities[i];
                    NewGenerator->GeneratorData.Income = DataToLoad->Gens.Incomes[i];
                    NewGenerator->GeneratorData.MoneyCost = DataToLoad->Gens.MoneyCosts[i];
                    NewGenerator->GeneratorData.ProductCost = DataToLoad->Gens.ProductCosts[i];
                    NewGenerator->GeneratorData.MaxTime = DataToLoad->Gens.MaxTimes[i];
                    NewGenerator->GeneratorData.SpeedPrice = DataToLoad->Gens.SpeedPrices[i];
                    NewGenerator->GeneratorData.GeneratorBought = DataToLoad->Gens.GeneratorsBought[i];
                    NewGenerator->GeneratorData.AmountOfGeneratorToBuy = DataToLoad->Gens.AmountOfGeneratorsToBuy[i];

                    if (i > 0)
                    {
                        NewGenerator->Product = Generators[i - 1];
                    }

                    NewGenerator->CheckIfBuyAmountHasBeenReached();

                    // Add the new generator to the Generators array
                    Generators.Add(NewGenerator);
                }
                
                if (NewGenerator->GeneratorData.Quantity <= FLargeNumber(0.0, 0))
                {
                    FLinearColor CustomColor(.1, .1, .1, 1.0);
                    NewGenerator->GeneratorBackground->SetBrushColor(CustomColor);
                    NewGenerator->GeneratorBackground->SetIsEnabled(false);
                    NewGenerator->GeneratorButton->SetIsEnabled(false);
                }
            }
        }
        else
        {
            InitGenerators();
        }
    }
    else
    {
        CreateSaveFile();
        UE_LOG(LogTemp, Warning, TEXT("Load"))
    }
}

TArray<FManagerData> UMainGameInstance::GetAllManagers() const
{
    TArray<FManagerData> Managers;
    if (ManagerDataTable)
    {
        TArray<FName> RowNames = ManagerDataTable->GetRowNames();
        for (auto& Name : RowNames)
        {
            FManagerData* Row = ManagerDataTable->FindRow<FManagerData>(Name, TEXT(""));
            if (Row)
            {
                Managers.Add(*Row);
            }
        }
    }
    return Managers;
}

FManagerData* UMainGameInstance::GetManagerByName(FName ManagerName) const
{
    if (ManagerDataTable)
    {
        return ManagerDataTable->FindRow<FManagerData>(ManagerName, TEXT(""));
    }
    return nullptr;
}


void UMainGameInstance::Init()
{
	Super::Init();

    WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();
    LoadGame();
    
}

void UMainGameInstance::OnStart()
{
    Super::OnStart();
}

void UMainGameInstance::Shutdown()
{
    Super::Shutdown();
    SaveGame();
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "IdleGameSave.h"
#include "UI/GeneratorUI.h"
#include "MainWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"


double UMainGameInstance::CalculateOfflineTime()
{
    // Calculate offline time
    FString SaveFilePath = FPaths::ProjectSavedDir() + TEXT("SaveGames/Slot1.sav");
    DataToLoad->SaveTime = IFileManager::Get().GetTimeStamp(*SaveFilePath);
    
    FDateTime CurrentTime = FDateTime::UtcNow();
    double TimeElapsed = (CurrentTime - DataToLoad->SaveTime).GetTotalSeconds();
    double MaxOfflineTime = 43200; // Maximum offline time in seconds (12 hours)

    // UE_LOG(LogTemp, Warning, TEXT("TimeElapsed: %d"), DataToLoad->SaveTime)
    //UE_LOG(LogTemp, Warning, TEXT("TimeElapsed: %.0LF"), TimeElapsed)

    // Ensure the calculated offline time doesn't exceed the maximum allowed time
    TimeElapsed = FMath::Min(TimeElapsed, MaxOfflineTime);
    TimeElapsed = FMath::Max(TimeElapsed, 0.0); // Ensure time elapsed is not negative
    
    //UE_LOG(LogTemp, Warning, TEXT("TimeElapsed: %.0LF"), TimeElapsed)
    
    return TimeElapsed;
}

UMainGameInstance::UMainGameInstance()
{
    
}

void UMainGameInstance::InitGenerators()
{
    // Clear existing generators
    Generators.Empty();

    for (int32 i = 0; i < NumberOfGenerators; i++)
    {
        UGeneratorUI* NewGenerator = CreateWidget<UGeneratorUI>(GetWorld(), GeneratorUIClass);
        NewGenerator->GeneratorData.Income = (i == 0) ? 1 : (i + 1) * 1;
        NewGenerator->GeneratorData.Quantity = (i == 0) ? 1 : 0;
        NewGenerator->GeneratorData.MoneyCost = (i == 0) ? 10 : (i + 1) * 5 * 8 * 10;
        NewGenerator->GeneratorData.ProductCost = (i == 0) ? 0 : i * (3 + i);
        NewGenerator->GeneratorData.MaxTime = (i == 0) ? 1 : i * 2;
        NewGenerator->GeneratorData.GeneratorName = FString::Printf(TEXT("Gen %d"), i + 1);
        NewGenerator->GeneratorData.SpeedPrice = 1;

        if(i > 0)
        {
            NewGenerator->Product = Generators[i - 1];
        }
        
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
        
        for (int32 i = 0; i < Generators.Num(); i++)
        {
            UGeneratorUI* CurrentGenerator = Generators[i];
            if (IsValid(CurrentGenerator))
            {
                DataToSave->Gens.GeneratorNames.Add(Generators[i]->GeneratorData.GeneratorName);
                DataToSave->Gens.Quantities.Add(Generators[i]->GeneratorData.Quantity);
                DataToSave->Gens.Incomes.Add(Generators[i]->GeneratorData.Income);
                DataToSave->Gens.MoneyCosts.Add(Generators[i]->GeneratorData.MoneyCost);
                DataToSave->Gens.ProductCosts.Add(Generators[i]->GeneratorData.ProductCost);
                DataToSave->Gens.MaxTimes.Add(Generators[i]->GeneratorData.MaxTime);
                DataToSave->Gens.SpeedPrices.Add(Generators[i]->GeneratorData.SpeedPrice);

                // UE_LOG(LogTemp, Warning, TEXT("saved gen quantity: %.0LF"), DataToSave->Gens.Quantities[i])
            }
        }

        //UE_LOG(LogTemp, Warning, TEXT("Generator 5: %s"), *DataToSave->Gen4->GeneratorData.GeneratorName);
        
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
        
        double TotalOfflineTime = CalculateOfflineTime();

        if(DataToLoad->Gens.GeneratorNames.Num() > 0)
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

                    if(i > 0)
                    {
                        NewGenerator->Product = Generators[i - 1];
                    }

                    if (NewGenerator->GeneratorData.Quantity > 0)
                    {
                        // Calculate the income generated during the offline time and add it immediately
                        double TimesTriggered = TotalOfflineTime / NewGenerator->GeneratorData.MaxTime;
                        if (NewGenerator->Product)
                        {
                            NewGenerator->Product->GeneratorData.Quantity += NewGenerator->GeneratorData.Income * NewGenerator->GeneratorData.Quantity * TimesTriggered;
                        }
                        else
                        {
                            Money += NewGenerator->GeneratorData.Income * NewGenerator->GeneratorData.Quantity * TimesTriggered;
                        }
                    }

                    // UE_LOG(LogTemp, Warning, TEXT("loaded gen quantity: %.0LF"), NewGenerator->GeneratorData.Quantity)
                
                    // Add the new generator to the Generators array
                    Generators.Add(NewGenerator);
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

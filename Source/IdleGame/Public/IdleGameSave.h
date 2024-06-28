// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainWorldSubsystem.h"
#include "GameFramework/SaveGame.h"
#include "IdleGameSave.generated.h"

class UGeneratorUI;

USTRUCT(BlueprintType)
struct FGeneratorSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FString> GeneratorNames;

	UPROPERTY(VisibleAnywhere)
	TArray<FLargeNumber> Quantities;

	UPROPERTY(VisibleAnywhere)
	TArray<FLargeNumber> Incomes;

	UPROPERTY(VisibleAnywhere)
	TArray<FLargeNumber> MoneyCosts;

	UPROPERTY(VisibleAnywhere)
	TArray<FLargeNumber> ProductCosts;

	UPROPERTY(VisibleAnywhere)
	TArray<FLargeNumber> MaxTimes;

	UPROPERTY(VisibleAnywhere)
	TArray<FLargeNumber> SpeedPrices;

	UPROPERTY(VisibleAnywhere)
	TArray<FLargeNumber> GeneratorsBought;

	UPROPERTY(VisibleAnywhere)
	TArray<FLargeNumber> AmountOfGeneratorsToBuy;
};

UCLASS()
class IDLEGAME_API UIdleGameSave : public USaveGame
{
	GENERATED_BODY()

public:
	UIdleGameSave();

	FDateTime SaveTime;
	
	UPROPERTY(VisibleAnywhere)
	FLargeNumber Money;

	UPROPERTY(VisibleAnywhere)
	FGeneratorSaveData Gens;
	

	
	
};



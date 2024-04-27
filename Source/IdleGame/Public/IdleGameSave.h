// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UI/GeneratorUI.h"
#include "IdleGameSave.generated.h"

class UGeneratorUI;

USTRUCT(BlueprintType)
struct FGeneratorSaveData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FString> GeneratorNames;

	UPROPERTY(VisibleAnywhere)
	TArray<double> Quantities;

	UPROPERTY(VisibleAnywhere)
	TArray<double> Incomes;

	UPROPERTY(VisibleAnywhere)
	TArray<double> MoneyCosts;

	UPROPERTY(VisibleAnywhere)
	TArray<double> ProductCosts;

	UPROPERTY(VisibleAnywhere)
	TArray<double> MaxTimes;
};

UCLASS()
class IDLEGAME_API UIdleGameSave : public USaveGame
{
	GENERATED_BODY()

public:
	UIdleGameSave();

	FDateTime SaveTime;
	
	UPROPERTY(VisibleAnywhere)
	double Money;

	UPROPERTY(VisibleAnywhere)
	FGeneratorSaveData Gens;
	

	
	
};



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MainUI.h"
#include "MainGameInstance.generated.h"

class UIdleGameSave;
class UMainWorldSubsystem;
UCLASS()
class IDLEGAME_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY()
	UMainWorldSubsystem* WorldSubsystem;
	
	

public:
	UMainGameInstance();

	UPROPERTY()
	UIdleGameSave* DataToSave;

	UPROPERTY()
	UIdleGameSave* DataToLoad;

	void CreateSaveFile();
	void SaveGame();
	void LoadGame();
	virtual void Init() override;
	
	virtual void Shutdown() override;

	double CalculateOfflineTime();
	
	UPROPERTY()
	double Money = 1;

	UPROPERTY()
	double StartMoney;

	UPROPERTY(VisibleAnywhere)
	TArray<UGeneratorUI*> Generators;

	UPROPERTY(EditAnywhere, Category = "Generator")
	int32 NumberOfGenerators = 10;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UGeneratorUI> GeneratorUIClass;

	void InitGenerators();

	FString CurrentSelectedGenerator = "None";

	bool bIsBought;
	
protected:
	virtual void OnStart() override;
};

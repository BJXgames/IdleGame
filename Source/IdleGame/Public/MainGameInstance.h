// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainWorldSubsystem.h"
#include "Engine/GameInstance.h"
#include "UI/MainUI.h"
#include "MainGameInstance.generated.h"

class UIdleGameSave;

UCLASS()
class IDLEGAME_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY()
	AMainPlayerController* MainPlayerController;

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

	float CalculateOfflineTime();
	
	UPROPERTY()
	FLargeNumber Money;

	UPROPERTY()
	FLargeNumber StartMoney;

	UPROPERTY(VisibleAnywhere)
	TArray<UGeneratorUI*> Generators;

	UPROPERTY(EditAnywhere, Category = "Generator")
	int32 NumberOfGenerators = 10;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UGeneratorUI> GeneratorUIClass;

	void InitGenerators();

	UPROPERTY()
	UGeneratorUI* CurrentSelectedGenerator;

	bool bIsBought;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Managers")
	UDataTable* ManagerDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Managers")
	UDataTable* ManagersUnlockedDataTable;

	UFUNCTION(BlueprintCallable, Category = "Managers")
	void AddManagerToInventory();

	UFUNCTION(BlueprintCallable)
	void InitializeWithPlayerController(AMainPlayerController* PlayerController);

protected:
	virtual void OnStart() override;

	UPROPERTY(EditAnywhere)
	UTexture2D* Image;

	const FManagerData* GetRandomManagerFromDataTable();
};

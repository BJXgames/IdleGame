// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GeneratorUI.generated.h"

class AMainPlayerController;
class UMainGameInstance;
class UButton;
class UMainWorldSubsystem;
class UTextBlock;
class UProgressBar;

USTRUCT(BlueprintType)
struct FGeneratorData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	double Quantity;
	
	UPROPERTY(VisibleAnywhere)
	double Income;
	
	UPROPERTY(VisibleAnywhere)
	double MoneyCost;
	
	UPROPERTY(VisibleAnywhere)
	double ProductCost;
	
	UPROPERTY(VisibleAnywhere)
	double MaxTime;

	UPROPERTY(VisibleAnywhere)
	double SpeedPrice;
	
	UPROPERTY(VisibleAnywhere)
	FString GeneratorName;
};

UCLASS()
class IDLEGAME_API UGeneratorUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UMainWorldSubsystem* WorldSubsystem;
	
	UPROPERTY()
	UMainGameInstance* MainGameInstance;

	UPROPERTY()
	AMainPlayerController* PlayerController;

public:
	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* QuantityDisplay;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* IncomeDisplay;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* NameDisplay;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* MoneyCostDisplay;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* ProductCostDisplay;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UButton* BuyButton;
	
	FGeneratorData GeneratorData;

	UPROPERTY(VisibleAnywhere, Category = "Generator")
	UGeneratorUI* Product;

	UPROPERTY(VisibleAnywhere, Category = "Generator")
	double Time;

	void GenerateIncome();

	UFUNCTION(BlueprintCallable)
	void Buy();

	UPROPERTY(EditAnywhere, Category = "Multiplier")
	double BuyMultiplier = 1;

	UPROPERTY()
	double GeneratorCostMultiplier = 1.21;

	double MoneyCost;
	

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	
	void UpdateUIDisplays();
	void CheckIncomeGeneration();
	void UpdateProgressBar();
	void UpdateBuyButtonState();

	UFUNCTION(BlueprintCallable)
	void OpenUpgradeWidget();

	
	
	

	
};

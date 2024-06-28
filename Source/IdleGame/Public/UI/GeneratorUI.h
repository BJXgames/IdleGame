// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainWorldSubsystem.h"
#include "UpgradeUI.h"
#include "Blueprint/UserWidget.h"
#include "Data/ManagerData.h"
#include "GeneratorUI.generated.h"

class AMainPlayerController;
class UMainGameInstance;
class UButton;
class UMainWorldSubsystem;
class UTextBlock;
class UProgressBar;
class UBorder;

USTRUCT(BlueprintType)
struct FGeneratorData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FLargeNumber Quantity;
	
	UPROPERTY(VisibleAnywhere)
	FLargeNumber Income;
	
	UPROPERTY(VisibleAnywhere)
	FLargeNumber MoneyCost;
	
	UPROPERTY(VisibleAnywhere)
	FLargeNumber ProductCost;
	
	UPROPERTY(VisibleAnywhere)
	FLargeNumber MaxTime;

	UPROPERTY(VisibleAnywhere)
	FLargeNumber SpeedPrice;

	UPROPERTY(VisibleAnywhere)
	FLargeNumber GeneratorBought;

	UPROPERTY(VisibleAnywhere)
	FLargeNumber AmountOfGeneratorToBuy;
	
	UPROPERTY(VisibleAnywhere)
	FString GeneratorName;

	UPROPERTY(VisibleAnywhere)
	FManagerData ManagerData;
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
	UTextBlock* AmountAddedToQuantityDisplay;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* AmountBoughtOfTheGeneratorDisplay;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* AmountToReachIfBuyingGeneratorDisplay;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UButton* BuyButton;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UButton* GeneratorButton;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UBorder* GeneratorBackground;
	
	FGeneratorData GeneratorData;

	UPROPERTY(VisibleAnywhere, Category = "Generator")
	UGeneratorUI* Product;

	UPROPERTY(VisibleAnywhere, Category = "Generator")
	double Time;

	void GenerateIncome();

	UFUNCTION(BlueprintCallable)
	void Buy();

	UPROPERTY(EditAnywhere, Category = "Multiplier")
	FLargeNumber BuyMultiplier = FLargeNumber(1.0, 0);

	UPROPERTY()
	FLargeNumber GeneratorCostMultiplier = 1.21;

	FLargeNumber MoneyCost;
	FLargeNumber ProductCost;

	UFUNCTION(BlueprintCallable)
	void ToggleUpgradeWidget(UWidgetAnimation* Animation);

	void UpdateBuyButtonState();

	void CheckIfBuyAmountHasBeenReached();

	UFUNCTION(BlueprintCallable)
	void SelectGenerator();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	
	void UpdateUIDisplays();
	void CheckIncomeGeneration();
	void UpdateProgressBar();

	UPROPERTY(BlueprintReadOnly)
	UUpgradeUI* UpgradeUIWidget;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHiding;

	
	

	
	
	

	
};

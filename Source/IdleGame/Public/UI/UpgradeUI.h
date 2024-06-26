// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeUI.generated.h"

class UGeneratorUI;
class UMainGameInstance;
class UMainWorldSubsystem;
class UTextBlock;
class AMainPlayerController;
UCLASS()
class IDLEGAME_API UUpgradeUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UMainWorldSubsystem* WorldSubsystem;
	
	UPROPERTY()
	UMainGameInstance* MainGameInstance;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* SpeedUpgradePriceText;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* GenNameText;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* GenQuantityText;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* GenIncomeText;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* GenSpeedText;

public:
	void UpdateGenText(double Quantity, double MaxTime, double Income, FString GenName);

	UFUNCTION(BlueprintCallable)
	void UpgradeSpeed();

	UPROPERTY()
	UGeneratorUI* CurrentGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetAnimation* ToggleUpgradeWidgetAnimation;

	
	
};

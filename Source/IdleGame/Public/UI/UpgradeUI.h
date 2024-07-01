// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainWorldSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeUI.generated.h"

class UManagerUI;
class UButton;
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

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UButton* SpeedUpgradeButton;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UManagerUI* BP_ManagerWidget;

	const int32 MaxSpeedUpgrades = 15; // Maximum number of speed upgrades

public:
	void UpdateGenText(FLargeNumber Quantity, FLargeNumber MaxTime, FLargeNumber Income, FString GenName);

	UFUNCTION(BlueprintCallable)
	void UpgradeSpeed();

	UPROPERTY()
	UGeneratorUI* CurrentGenerator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetAnimation* ToggleUpgradeWidgetAnimation;

	UFUNCTION(BlueprintCallable, Category = "UI")
	UManagerUI* GetManagerUI() const;
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

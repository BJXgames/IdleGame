// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneratorUI.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"


class UUniformGridPanel;
class UMainGameInstance;
class UScrollBox;
class UGeneratorUI;
class UTextBlock;
class UMainWorldSubsystem;



UCLASS()
class IDLEGAME_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UMainWorldSubsystem* WorldSubsystem;

	UPROPERTY()
	UMainGameInstance* MainGameInstance;

	UPROPERTY()
	AMainPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UTextBlock* CurrentMoneyText;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UTextBlock* IncomePerSecondText;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UUniformGridPanel* UniformGrid;

protected:
	
	UFUNCTION(BlueprintCallable)
	void DeleteSave();

	UFUNCTION(BlueprintCallable)
	void AddTime();
	
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void AddGeneratorToTheScrollBox();

	void UpdateIncomePerSecond();

	bool bIsbuyx1Active = true;
	bool bIsbuyx5Active = false;
	bool bIsbuyx10Active = false;
	bool bIsbuyMaxActive = false;

	FLargeNumber IncomePerSecond;
	
public:
	
	UFUNCTION(BlueprintCallable)
	void buyx1();

	UFUNCTION(BlueprintCallable)
	void buyx5();

	UFUNCTION(BlueprintCallable)
	void buyx10();

	UFUNCTION(BlueprintCallable)
	void buyxMax();

	void UpdateBuyMultiplier();
	
};

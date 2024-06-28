// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainWorldSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "AfkUI.generated.h"

class UBackgroundBlur;
class UBorder;
class UMainGameInstance;
class UMainWorldSubsystem;
class AMainPlayerController;
class UTextBlock;
UCLASS()
class IDLEGAME_API UAfkUI : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void CloseWidget();

	void UpdateText();

	UPROPERTY()
	UMainWorldSubsystem* WorldSubsystem;

	UPROPERTY()
	UMainGameInstance* MainGameInstance;

	UPROPERTY()
	AMainPlayerController* PlayerController;
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* AfkTimeText;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* OfflineEarningText;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UBorder* MainBorder;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UBackgroundBlur* BackgroundBlur;

	FLargeNumber OfflineEarningAmount = FLargeNumber(0.0, 0);
	
};

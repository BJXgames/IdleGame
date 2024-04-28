// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeUI.generated.h"

class UMainWorldSubsystem;
class UTextBlock;
class AMainPlayerController;
UCLASS()
class IDLEGAME_API UUpgradeUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UMainWorldSubsystem* WorldSubsystem;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* SpeedUpgradePrice;

	void UpdateText();

	
	
};

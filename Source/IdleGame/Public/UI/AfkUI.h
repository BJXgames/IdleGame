// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AfkUI.generated.h"

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
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* AfkTimeText;

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UTextBlock* OfflineEarningText;

	UPROPERTY()
	AMainPlayerController* PlayerController;
	
};

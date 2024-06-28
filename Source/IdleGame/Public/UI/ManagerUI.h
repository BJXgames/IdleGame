// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ManagerUI.generated.h"

class UGeneratorUI;
class UManagerPanelUI;
class AMainPlayerController;
class UMainGameInstance;
class UMainWorldSubsystem;
class UBorder;
UCLASS()
class IDLEGAME_API UManagerUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UMainWorldSubsystem* WorldSubsystem;

	UPROPERTY()
	UMainGameInstance* MainGameInstance;

	UPROPERTY()
	AMainPlayerController* PlayerController;
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UBorder* ManagerImage;

	UFUNCTION(BlueprintCallable)
	void OpenManagerPanel();

public:
	void UpdateManagerImage(UTexture2D* Image);
	
};

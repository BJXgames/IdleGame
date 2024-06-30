// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ManagerData.h"
#include "ManagerPanelUI.generated.h"

class UWrapBox;
class AMainPlayerController;
class UMainGameInstance;
class UMainWorldSubsystem;
class UGeneratorUI;
class UImage;
class UTextBlock;
class UManagersInScrollBoxUI;
UCLASS()
class IDLEGAME_API UManagerPanelUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UMainWorldSubsystem* WorldSubsystem;

	UPROPERTY()
	UMainGameInstance* MainGameInstance;

	UPROPERTY()
	AMainPlayerController* PlayerController;
	

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UWrapBox* WrapBox;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UTextBlock* ManagerNameText;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UTextBlock* SpeedBoostText;
 
	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UTextBlock* IncomeMultiplierText;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UTextBlock* MoneyPriceReductionText;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UImage* ManagerImage;
	

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UTextBlock* SpeedTextBlock;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UTextBlock* IncomeTextBlock;

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UTextBlock* PriceTextBlock;
	

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UManagersInScrollBoxUI>  ManagerInScrollBoxUIClass;

	TObjectPtr<UManagersInScrollBoxUI> ManagerInScrollBoxUI;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Managers", meta=(AllowPrivateAccess="true"))
	UDataTable* ManagersUnlockedDataTable;

	TWeakObjectPtr<UGeneratorUI> SelectedGenerator;
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void HideManagerInfo();
	void DisplayManagerInfo(const FManagerData& ManagerData);
	void UpdateGeneratorData(const FManagerData& ManagerData);
	void UpdateTextBlock(UTextBlock* TextBlock, const FLinearColor& Color);
	void UpdateManagerInfoText(UTextBlock* TextBlock, float Value);
	void AdjustWidgetPadding();

public:
	UFUNCTION()
	void UpdateGeneratorManager(UGeneratorUI* Generator);

	void ResetManagersPanelText();

	void UpdateManagersToGrid();
	

	UFUNCTION()
	void UpdateManagerInfo(FManagerData ManagerData);

	void SetUIElementVisibility(ESlateVisibility Visibility, const TArray<UTextBlock*>& Elements);
	
};


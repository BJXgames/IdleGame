// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ManagerPanelUI.generated.h"

class UImage;
class UTextBlock;
class UUniformGridPanel;
class UManagersInScrollBoxUI;
UCLASS()
class IDLEGAME_API UManagerPanelUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UUniformGridPanel* UniformGridPanel;

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

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UManagersInScrollBoxUI>  ManagerInScrollBoxUIClass;

	TObjectPtr<UManagersInScrollBoxUI> ManagerInScrollBoxUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Managers", meta=(AllowPrivateAccess="true"))
	UDataTable* ManagerDataTable;

	void AddManagersToUniformGrid();

	UFUNCTION()
	void UpdateManagerInfo(const FName& ManagerName, float SpeedBoost, float IncomeMultiplier, float MoneyPriceReduction, UTexture2D* Image);
	
protected:
	virtual void NativeConstruct() override;
};

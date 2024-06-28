// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ManagerPanelUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Data/ManagerData.h"
#include "UI/ManagersInScrollBoxUI.h"


void UManagerPanelUI::NativeConstruct()
{
	Super::NativeConstruct();

	AddManagersToUniformGrid();
	if(ManagerImage)
	{
		FSlateColor TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
		ManagerImage->SetBrushTintColor(TintColor);
	}
}

void UManagerPanelUI::AddManagersToUniformGrid()
{
	if (!ManagerDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Manager DataTable not set"));
		return;
	}

	int32 CurrentRow = 0;
	int32 CurrentColumn = 0;
	const int32 MaxColumns = 5;

	TArray<FName> RowNames = ManagerDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FManagerData* ManagerData = ManagerDataTable->FindRow<FManagerData>(RowName, TEXT(""));
		if (ManagerData)
		{
			UManagersInScrollBoxUI* ManagerWidget = CreateWidget<UManagersInScrollBoxUI>(GetWorld(), ManagerInScrollBoxUIClass);
			if (ManagerWidget)
			{
				ManagerWidget->InitializeManager(*ManagerData);

				// Bind the OnManagerSelected
				ManagerWidget->OnManagerSelected.AddDynamic(this, &UManagerPanelUI::UpdateManagerInfo);

				UniformGridPanel->AddChildToUniformGrid(ManagerWidget, CurrentRow, CurrentColumn);

				ManagerWidget->SetPadding(FMargin(25.0f, 25.0f, 0.0f, 0.0f));

				// Update the column and row
				++CurrentColumn;
				if (CurrentColumn >= MaxColumns)
				{
					CurrentColumn = 0;
					++CurrentRow;
				}
			}
		}
	}
}

void UManagerPanelUI::UpdateManagerInfo(const FName& ManagerName, float SpeedBoost, float IncomeMultiplier, float MoneyPriceReduction, UTexture2D* Image)
{
	if (ManagerNameText)
	{
		ManagerNameText->SetText(FText::FromName(ManagerName));
	}
	if (SpeedBoostText)
	{
		FText SpeedBoostValue = FText::AsNumber(SpeedBoost);
		FText SpeedBoostDisplay = FText::Format(FText::FromString("{0}x"), SpeedBoostValue);
		SpeedBoostText->SetText(SpeedBoostDisplay);
	}
	if (IncomeMultiplierText)
	{
		FText IncomeMultiplierValue = FText::AsNumber(IncomeMultiplier);
		FText IncomeMultiplierDisplay = FText::Format(FText::FromString("{0}x"), IncomeMultiplierValue);
		IncomeMultiplierText->SetText(IncomeMultiplierDisplay);
	}
	if (MoneyPriceReductionText)
	{
		FText MoneyPriceReductionValue = FText::AsNumber(MoneyPriceReduction);
		FText MoneyPriceReductionDisplay = FText::Format(FText::FromString("{0}x"), MoneyPriceReductionValue);
		MoneyPriceReductionText->SetText(MoneyPriceReductionDisplay);
	}
	if(ManagerImage)
	{
		FSlateColor TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		ManagerImage->SetBrushTintColor(TintColor);
		ManagerImage->SetBrushFromTexture(Image);
	}
}

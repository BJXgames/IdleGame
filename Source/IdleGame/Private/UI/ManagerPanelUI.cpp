// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ManagerPanelUI.h"

#include "MainWorldSubsystem.h"
#include "MainPlayerController.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Data/ManagerData.h"
#include "UI/GeneratorUI.h"
#include "UI/ManagersInScrollBoxUI.h"
#include "UI/ManagerUI.h"
#include "MainGameInstance.h"


void UManagerPanelUI::NativeConstruct()
{
	Super::NativeConstruct();

	MainGameInstance = GetGameInstance<UMainGameInstance>();
	PlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
	WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();

	AddManagersToUniformGrid();
}

void UManagerPanelUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
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
	if(Image == nullptr)
	{
		FSlateColor TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
		ManagerImage->SetBrushTintColor(TintColor);
	}
	if (SelectedGenerator.IsValid())
	{
		SelectedGenerator->GeneratorData.ManagerData.Name = ManagerName;
		SelectedGenerator->GeneratorData.ManagerData.SpeedBoost = SpeedBoost;
		SelectedGenerator->GeneratorData.ManagerData.IncomeMultiplier = IncomeMultiplier;
		SelectedGenerator->GeneratorData.ManagerData.MoneyPriceReduction = MoneyPriceReduction;
		SelectedGenerator->GeneratorData.ManagerData.ManagerImage = Image;
	}
	if (ManagerNameText)
	{
		ManagerNameText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ManagerNameText->SetText(FText::FromName(ManagerName));
	}
	if (SpeedBoostText)
	{
		SpeedBoostText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FText SpeedBoostValue = FText::AsNumber(SpeedBoost);
		FText SpeedBoostDisplay = FText::Format(FText::FromString("{0}x"), SpeedBoostValue);
		SpeedBoostText->SetText(SpeedBoostDisplay);
	}
	if (IncomeMultiplierText)
	{
		IncomeMultiplierText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FText IncomeMultiplierValue = FText::AsNumber(IncomeMultiplier);
		FText IncomeMultiplierDisplay = FText::Format(FText::FromString("{0}x"), IncomeMultiplierValue);
		IncomeMultiplierText->SetText(IncomeMultiplierDisplay);
	}
	if (MoneyPriceReductionText)
	{
		MoneyPriceReductionText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FText MoneyPriceReductionValue = FText::AsNumber(MoneyPriceReduction);
		FText MoneyPriceReductionDisplay = FText::Format(FText::FromString("{0}x"), MoneyPriceReductionValue);
		MoneyPriceReductionText->SetText(MoneyPriceReductionDisplay);
	}
	if (ManagerImage)
	{
		FSlateColor TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		ManagerImage->SetBrushTintColor(TintColor);
		ManagerImage->SetBrushFromTexture(Image);
	}

	PlayerController->GetUpgradeUI()->GetManagerUI()->UpdateManagerImage(Image);
	
}

void UManagerPanelUI::UpdateGeneratorManager(UGeneratorUI* Generator)
{
	SelectedGenerator = Generator;

	UpdateManagerInfo(Generator->GeneratorData.ManagerData.Name,
		Generator->GeneratorData.ManagerData.SpeedBoost,
		Generator->GeneratorData.ManagerData.IncomeMultiplier,
		Generator->GeneratorData.ManagerData.MoneyPriceReduction,
		Generator->GeneratorData.ManagerData.ManagerImage);
}


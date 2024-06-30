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
#include "Components/HorizontalBoxSlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

void UManagerPanelUI::NativeConstruct()
{
    Super::NativeConstruct();

    MainGameInstance = GetGameInstance<UMainGameInstance>();
    PlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
    WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();

    UpdateManagersToGrid();
}

void UManagerPanelUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    static FVector2D LastSize = FVector2D::ZeroVector;
    FVector2D CurrentSize = WrapBox->GetCachedGeometry().GetLocalSize();

    if (!CurrentSize.Equals(LastSize))
    {
        LastSize = CurrentSize;
        AdjustWidgetPadding(); // Adjust padding when the WrapBox size changes
    }
}

void UManagerPanelUI::UpdateManagersToGrid()
{
    if (!ManagersUnlockedDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Managers unlocked DataTable not set"));
        return;
    }

    WrapBox->ClearChildren(); // Clear existing children

    const TArray<FName> RowNames = ManagersUnlockedDataTable->GetRowNames();
    if (RowNames.Num() == 0)
    {
        return; // No managers to display
    }

    for (int32 i = 0; i < RowNames.Num(); ++i)
    {
        FName RowName = RowNames[i];
        FManagerData* ManagerData = ManagersUnlockedDataTable->FindRow<FManagerData>(RowName, TEXT(""));
        if (ManagerData)
        {
            UManagersInScrollBoxUI* ManagerWidget = CreateWidget<UManagersInScrollBoxUI>(GetWorld(), ManagerInScrollBoxUIClass);
            if (ManagerWidget)
            {
                ManagerWidget->InitializeManager(*ManagerData);
                ManagerWidget->OnManagerSelected.AddDynamic(this, &UManagerPanelUI::UpdateManagerInfo);

                // Add widget to wrap box with default padding
                WrapBox->AddChildToWrapBox(ManagerWidget);
            }
        }
    }

    AdjustWidgetPadding();
}

void UManagerPanelUI::AdjustWidgetPadding()
{
    static FVector2D LastSize = FVector2D::ZeroVector;
    FVector2D CurrentSize = WrapBox->GetCachedGeometry().GetLocalSize();
    
    if (CurrentSize.Equals(LastSize))
    {
        return;
    }

    
    const float TotalWrapBoxWidth = WrapBox->GetCachedGeometry().GetLocalSize().X;
    const float WidgetWidth = 200.0f; // Assuming each widget has a fixed width
    const float DesiredPadding = 20.0f; // Minimum desired padding between widgets

    int32 WidgetsPerRow = FMath::FloorToInt(TotalWrapBoxWidth / (WidgetWidth + DesiredPadding));
    WidgetsPerRow = FMath::Max(WidgetsPerRow, 1); // Ensure at least one widget per row

    float TotalPaddingSpace = TotalWrapBoxWidth - (WidgetsPerRow * WidgetWidth);
    float PaddingBetweenWidgets = TotalPaddingSpace / (WidgetsPerRow + 1);

    const TArray<UWidget*>& Children = WrapBox->GetAllChildren();
    for (int32 i = 0; i < Children.Num(); ++i)
    {
        UWrapBoxSlot* WrapBoxSlot = Cast<UWrapBoxSlot>(Children[i]->Slot);
        if (WrapBoxSlot)
        {
            float LeftPadding = PaddingBetweenWidgets;

            // Apply the same padding to all widgets
            WrapBoxSlot->SetPadding(FMargin(LeftPadding, 25.0f, 0.0f, 0.0f));
            WrapBoxSlot->SetHorizontalAlignment(HAlign_Left);
        }
    }
}


void UManagerPanelUI::UpdateManagerInfo(FManagerData ManagerData)
{
    if (ManagerData.ManagerImage == nullptr)
    {
        HideManagerInfo();
    }
    else
    {
        DisplayManagerInfo(ManagerData);
    }

    // Update the manager image in the ManagerUI of the UpgradeUI
    PlayerController->GetUpgradeUI()->GetManagerUI()->UpdateManagerImage(ManagerData.ManagerImage);
}

void UManagerPanelUI::HideManagerInfo()
{
    const FSlateColor HiddenColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
    ManagerImage->SetBrushTintColor(HiddenColor);

    SetUIElementVisibility(ESlateVisibility::Collapsed, { ManagerNameText, SpeedBoostText, IncomeMultiplierText, MoneyPriceReductionText });
}

void UManagerPanelUI::DisplayManagerInfo(const FManagerData& ManagerData)
{
    const FLinearColor RarityColor = ManagerData.GetRarityColor();

    UpdateGeneratorData(ManagerData);

    if (ManagerNameText)
    {
        ManagerNameText->SetColorAndOpacity(RarityColor);
        ManagerNameText->SetText(FText::FromName(ManagerData.Name));
        ManagerNameText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }

    UpdateTextBlock(SpeedTextBlock, RarityColor);
    UpdateTextBlock(IncomeTextBlock, RarityColor);
    UpdateTextBlock(PriceTextBlock, RarityColor);

    UpdateManagerInfoText(SpeedBoostText, ManagerData.SpeedBoost);
    UpdateManagerInfoText(IncomeMultiplierText, ManagerData.IncomeMultiplier);
    UpdateManagerInfoText(MoneyPriceReductionText, ManagerData.MoneyPriceReduction);

    if (ManagerImage)
    {
        const FSlateColor VisibleColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
        ManagerImage->SetBrushTintColor(VisibleColor);
        ManagerImage->SetBrushFromTexture(ManagerData.ManagerImage);
    }
}

void UManagerPanelUI::UpdateGeneratorData(const FManagerData& ManagerData)
{
    if (SelectedGenerator.IsValid())
    {
        auto& GenData = SelectedGenerator->GeneratorData;
        GenData.ManagerData = ManagerData;

        const float AdjustedMaxTime = GenData.ManagerData.ManagerImage ? GenData.MaxTime.Value / GenData.ManagerData.SpeedBoost : GenData.MaxTime.Value;
        const FLargeNumber AdjustedIncome = GenData.ManagerData.ManagerImage ? GenData.Income * GenData.ManagerData.IncomeMultiplier : GenData.Income;

        PlayerController->GetUpgradeUI()->UpdateGenText(GenData.Quantity, AdjustedMaxTime, AdjustedIncome, GenData.GeneratorName);
    }
}

void UManagerPanelUI::UpdateTextBlock(UTextBlock* TextBlock, const FLinearColor& Color)
{
    if (TextBlock)
    {
        TextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        TextBlock->SetColorAndOpacity(Color);
    }
}

void UManagerPanelUI::UpdateManagerInfoText(UTextBlock* TextBlock, float Value)
{
    if (TextBlock)
    {
        TextBlock->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        TextBlock->SetText(FText::Format(FText::FromString("{0}x"), FText::AsNumber(Value)));
    }
}

void UManagerPanelUI::SetUIElementVisibility(ESlateVisibility VisibilityOfWidget, const TArray<UTextBlock*>& Elements)
{
    for (UTextBlock* Element : Elements)
    {
        if (Element)
        {
            Element->SetVisibility(VisibilityOfWidget);
        }
    }
}

void UManagerPanelUI::UpdateGeneratorManager(UGeneratorUI* Generator)
{
    SelectedGenerator = Generator;
    UpdateManagerInfo(Generator->GeneratorData.ManagerData);
}

void UManagerPanelUI::ResetManagersPanelText()
{
    const FLinearColor DefaultColor = FLinearColor::White;
    ManagerNameText->SetColorAndOpacity(DefaultColor);
    SpeedTextBlock->SetColorAndOpacity(DefaultColor);
    IncomeTextBlock->SetColorAndOpacity(DefaultColor);
    PriceTextBlock->SetColorAndOpacity(DefaultColor);
}
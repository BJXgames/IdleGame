// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AfkUI.h"

#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "MainWorldSubsystem.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UAfkUI::CloseWidget()
{
	MainBorder->SetVisibility(ESlateVisibility::Hidden);
	BackgroundBlur->SetVisibility(ESlateVisibility::Hidden);
	AfkTimeText->SetVisibility(ESlateVisibility::Hidden);
	MainGameInstance->Money += OfflineEarningAmount;
}

void UAfkUI::UpdateText()
{
	float OfflineTime = MainGameInstance->CalculateOfflineTime();
	int32 Days = OfflineTime / 86400;
	int32 Hours = (OfflineTime - (Days * 86400)) / 3600;
	int32 Minutes = (OfflineTime - (Days * 86400) - (Hours * 3600)) / 60;
	int32 Seconds = OfflineTime - (Days * 86400) - (Hours * 3600) - (Minutes * 60);

	FString TimeString = FString::Printf(TEXT("Time since last visit: %dh %dm %ds"), Hours, Minutes, Seconds);
	AfkTimeText->SetText(FText::FromString(TimeString));

	// Calculate offline earnings based on income rate and offline duration
	OfflineEarningAmount = MainGameInstance->Generators[0]->GeneratorData.Income * MainGameInstance->Generators[0]->GeneratorData.Quantity * OfflineTime; // Assuming GeneratorData.Income is per second
	OfflineEarningAmount.Normalize(); // Normalize the large number
    
	// Format and display offline earnings
	FString OfflineEarning = FString::Printf(TEXT("%s"), *WorldSubsystem->FormatLargeNumber(OfflineEarningAmount));
	OfflineEarningText->SetText(FText::FromString(OfflineEarning));
}

void UAfkUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UAfkUI::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AMainPlayerController>(GetOwningPlayer());
	MainGameInstance = GetGameInstance<UMainGameInstance>();
	WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();

	UpdateText();
}

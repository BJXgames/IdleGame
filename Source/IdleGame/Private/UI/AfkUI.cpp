// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AfkUI.h"

#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "MainWorldSubsystem.h"
#include "Components/TextBlock.h"

void UAfkUI::CloseWidget()
{
	PlayerController->AfkUI->SetVisibility(ESlateVisibility::Hidden);
}

void UAfkUI::UpdateText()
{
	double OflineTime = MainGameInstance->CalculateOfflineTime();
	int32 Days = OflineTime / 86400;
	int32 Hours = (OflineTime - (Days * 86400)) / 3600;
	int32 Minutes = (OflineTime - (Days * 86400) - (Hours * 3600)) / 60;
	int32 Seconds = OflineTime - (Days * 86400) - (Hours * 3600) - (Minutes * 60);
	
	FString TimeString = FString::Printf(TEXT("Time since last visit: %dh %dm %ds"), Hours, Minutes, Seconds);
	AfkTimeText->SetText(FText::FromString(TimeString));
	
	FString OfflineEarning = FString::Printf(TEXT("%s"), *WorldSubsystem->FormatLargeNumber(MainGameInstance->Money - MainGameInstance->StartMoney));
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UpgradeUI.h"
#include "MainWorldSubsystem.h"

void UUpgradeUI::NativeConstruct()
{
	Super::NativeConstruct();
	WorldSubsystem = GetWorld()->GetSubsystem<UMainWorldSubsystem>();
}

void UUpgradeUI::UpdateText()
{
	
}



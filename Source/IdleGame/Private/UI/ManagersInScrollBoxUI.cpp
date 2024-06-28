// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ManagersInScrollBoxUI.h"

#include "Components/Image.h"

void UManagersInScrollBoxUI::InitializeManager(const FManagerData& ManagerData)
{
	ManagerName = ManagerData.Name;
	SpeedBoost = ManagerData.SpeedBoost;
	IncomeMultiplier = ManagerData.IncomeMultiplier;
	MoneyPriceReduction = ManagerData.MoneyPriceReduction;
	ManagerImageInPanel = ManagerData.ManagerImage;
	
	if (ManagerImageInGrid && ManagerData.ManagerImage)
	{
		ManagerImageInGrid->SetBrushFromTexture(ManagerData.ManagerImage);
	}
}

void UManagersInScrollBoxUI::SelectManager()
{
	OnManagerSelected.Broadcast(ManagerName, SpeedBoost, IncomeMultiplier, MoneyPriceReduction, ManagerImageInPanel);
}

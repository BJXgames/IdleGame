// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ManagersInScrollBoxUI.h"
#include "UI/GeneratorUI.h"
#include "Components/Image.h"

void UManagersInScrollBoxUI::InitializeManager(const FManagerData& ManagerData)
{
	ManagerDataInScrollBox = ManagerData;
	
	if (ManagerImageInGrid && ManagerData.ManagerImage)
	{
		ManagerImageInGrid->SetBrushFromTexture(ManagerData.ManagerImage);
	}
}

void UManagersInScrollBoxUI::SelectManager()
{
	OnManagerSelected.Broadcast(ManagerDataInScrollBox);
}



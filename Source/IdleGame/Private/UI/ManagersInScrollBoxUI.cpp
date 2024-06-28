// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ManagersInScrollBoxUI.h"

#include "Components/Image.h"

void UManagersInScrollBoxUI::InitializeManager(const FManagerData& ManagerData)
{
	ManagerName = ManagerData.Name;
	if (ManagerImage && ManagerData.ManagerImage)
	{
		ManagerImage->SetBrushFromTexture(ManagerData.ManagerImage);
	}
}

void UManagersInScrollBoxUI::SelectManager()
{
	OnManagerSelected.Broadcast(ManagerName);
}

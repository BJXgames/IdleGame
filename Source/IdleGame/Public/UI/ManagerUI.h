// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ManagerUI.generated.h"

class UBorder;
UCLASS()
class IDLEGAME_API UManagerUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, Category = UI, meta = (BindWidget))
	UBorder* ManagerImage;
	
};

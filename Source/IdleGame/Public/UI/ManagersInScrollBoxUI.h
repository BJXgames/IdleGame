// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ManagerData.h"
#include "ManagersInScrollBoxUI.generated.h"

class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManagerSelected, const FName&, ManagerName);

UCLASS()
class IDLEGAME_API UManagersInScrollBoxUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	UImage* ManagerImage;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnManagerSelected OnManagerSelected;

	void InitializeManager(const FManagerData& ManagerData);

	UFUNCTION(BlueprintCallable)
	void SelectManager();

private:
	FName ManagerName;
};

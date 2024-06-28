// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UMainGameInstance;
class UMainUI;
class UAfkUI;
class UUpgradeUI;
class UManagerPanelUI;

UCLASS()
class IDLEGAME_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	UManagerPanelUI* GetManagerPanelUI() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUpgradeUI* GetUpgradeUI() const;


private:
	UPROPERTY()
	UMainGameInstance* MainGameInstance;

	UPROPERTY()
	UMainUI* MainUI;

	UPROPERTY()
	UAfkUI* AfkUI;

	UPROPERTY()
	UUpgradeUI* UpgradeUI;

	UPROPERTY()
	UManagerPanelUI* ManagerPanelUI;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UMainUI> MainUIClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UAfkUI> AfkUIClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUpgradeUI> UpgradeUIClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UManagerPanelUI> ManagerPanelUIClass;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UMainGameInstance;
class UUpgradeUI;
class UAfkUI;
class UMainUI;
UCLASS()
class IDLEGAME_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	UMainGameInstance* MainGameInstance;
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UMainUI>  MainUIClass;

	TObjectPtr<UMainUI> MainUI;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UAfkUI>  AfkUIClass;

	TObjectPtr<UAfkUI> AfkUI;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUpgradeUI>  UpgradeUIClass;

	UPROPERTY()
	UUpgradeUI* UpgradeUI;
};

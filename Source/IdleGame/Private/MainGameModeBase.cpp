// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"

#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeGameInstance();
}

void AMainGameModeBase::InitializeGameInstance()
{
	AMainPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<AMainPlayerController>();
	if (PlayerController)
	{
		UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
		if (GameInstance)
		{
			GameInstance->InitializeWithPlayerController(PlayerController);
		}
	}
}

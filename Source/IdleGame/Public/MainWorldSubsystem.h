// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UI/GeneratorUI.h"
#include "MainWorldSubsystem.generated.h"

// Define a struct to hold suffixes
struct FNumberSuffix
{
	FString Suffix;
	
};

// Define suffixes as static const array
static const TArray NumberSuffixes = {
	FNumberSuffix("K"),
	FNumberSuffix("M"),
	FNumberSuffix("B"),
	FNumberSuffix("Tr"),
	FNumberSuffix("Qu"),
	FNumberSuffix("Qi"),
	FNumberSuffix("Sx"),
	FNumberSuffix("Sp"),
	FNumberSuffix("O"),
	// Add more suffixes as needed 
};

// Define a struct to hold suffixes
struct FNumberPrefix
{
	FString Prefix;
	
};

// Define suffixes as static const array
static const TArray NumberPrefixes = {
	FNumberPrefix("U"),
	FNumberPrefix("D"),
	FNumberPrefix("T"),
	FNumberPrefix("QU"),
	FNumberPrefix("QI"),
	FNumberPrefix("SX"),
	// Add more suffixes as needed
};



UCLASS()
class IDLEGAME_API UMainWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	FString FormatLargeNumber(double Number);
};

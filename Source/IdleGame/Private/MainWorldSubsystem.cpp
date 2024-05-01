// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWorldSubsystem.h"


FString UMainWorldSubsystem::FormatLargeNumber(double Number)
{
	const double Threshold = 1000.0L;

	if (Number < 0)
	{
		// Handle negative numbers as needed
		return FString(TEXT("Invalid")); // Or any other appropriate action for negative numbers
	}

	// Find the appropriate suffix and format the number accordingly
	int32 MagnitudeIndex = 0;
	double FormattedNumber = Number;

	while (FormattedNumber >= Threshold && MagnitudeIndex < NumberSuffixes.Num())
	{
		FormattedNumber /= Threshold;
		MagnitudeIndex++;
	}

	// Format the number with the prefix and suffix
	FString FormattedString;
	if (MagnitudeIndex == 0)
	{
		// Use "%.0Lf" for whole numbers
		FormattedString = FString::Printf(TEXT("%.2Lf"), FormattedNumber);
	}
	else
	{
		// Use "%.1Lf" for numbers with one decimal place
		FormattedString = FString::Printf(TEXT("%.2Lf %s"), FormattedNumber, *NumberSuffixes[MagnitudeIndex - 1].Suffix);
	}

	return FormattedString;
}





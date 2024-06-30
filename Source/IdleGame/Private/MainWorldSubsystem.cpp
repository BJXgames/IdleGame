// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWorldSubsystem.h"

void FLargeNumber::Normalize()
{
	const float Threshold = 1000.0f;
	
	while (Value >= Threshold && MagnitudeIndex < NumberSuffixes.Num() - 1)
	{
		Value /= Threshold;
		++MagnitudeIndex;
	}
	while (Value < 1.0f && MagnitudeIndex > 0)
	{
		Value *= Threshold;
		--MagnitudeIndex;
	}
}

FLargeNumber FLargeNumber::operator+(const FLargeNumber& Other) const
{
	FLargeNumber Result = *this;
	Result += Other;
	return Result;
}

FLargeNumber FLargeNumber::operator-(const FLargeNumber& Other) const
{
	FLargeNumber Result = *this;
	Result -= Other;
	return Result;
}

FLargeNumber& FLargeNumber::operator+=(const FLargeNumber& Other)
{
	if (MagnitudeIndex == Other.MagnitudeIndex)
	{
		Value += Other.Value;
	}
	else if (MagnitudeIndex > Other.MagnitudeIndex)
	{
		Value += Other.Value / FMath::Pow(1000.0f, MagnitudeIndex - Other.MagnitudeIndex);
	}
	else
	{
		Value = (Value / FMath::Pow(1000.0f, Other.MagnitudeIndex - MagnitudeIndex)) + Other.Value;
		MagnitudeIndex = Other.MagnitudeIndex;
	}
	Normalize();
	return *this;
}

FLargeNumber& FLargeNumber::operator-=(const FLargeNumber& Other)
{
	if (MagnitudeIndex == Other.MagnitudeIndex)
	{
		Value -= Other.Value;
	}
	else if (MagnitudeIndex > Other.MagnitudeIndex)
	{
		Value -= Other.Value / FMath::Pow(1000.0f, MagnitudeIndex - Other.MagnitudeIndex);
	}
	else
	{
		Value = (Value / FMath::Pow(1000.0f, Other.MagnitudeIndex - MagnitudeIndex)) - Other.Value;
		MagnitudeIndex = Other.MagnitudeIndex;
	}
	Normalize();
	return *this;
}

bool FLargeNumber::operator>=(const FLargeNumber& Other) const
{
	if (MagnitudeIndex > Other.MagnitudeIndex)
	{
		return true;
	}
	if (MagnitudeIndex < Other.MagnitudeIndex)
	{
		return false;
	}
	return Value >= Other.Value;
}

bool FLargeNumber::operator<=(const FLargeNumber& Other) const
{
	if (MagnitudeIndex < Other.MagnitudeIndex)
	{
		return true;
	}
	if (MagnitudeIndex > Other.MagnitudeIndex)
	{
		return false;
	}
	return Value <= Other.Value;
}

bool FLargeNumber::operator>(const FLargeNumber& Other) const
{
	if (MagnitudeIndex > Other.MagnitudeIndex)
	{
		return true;
	}
	if (MagnitudeIndex < Other.MagnitudeIndex)
	{
		return false;
	}
	return Value > Other.Value;
}

bool FLargeNumber::operator<(const FLargeNumber& Other) const
{
	if (MagnitudeIndex < Other.MagnitudeIndex)
	{
		return true;
	}
	if (MagnitudeIndex > Other.MagnitudeIndex)
	{
		return false;
	}
	return Value < Other.Value;
}

bool FLargeNumber::operator==(const FLargeNumber& Other) const
{
	return MagnitudeIndex == Other.MagnitudeIndex && FMath::IsNearlyEqual(Value, Other.Value);
}

bool FLargeNumber::operator!=(const FLargeNumber& Other) const
{
	return !(*this == Other);
}

FLargeNumber FLargeNumber::operator*(float Multiplier) const
{
	FLargeNumber Result = *this;
	Result.Value *= Multiplier;
	Result.Normalize();
	return Result;
}

FLargeNumber& FLargeNumber::operator*=(float Multiplier)
{
	Value *= Multiplier;
	Normalize();
	return *this;
}

FLargeNumber FLargeNumber::operator*(const FLargeNumber& Other) const
{
	// Multiply values and adjust magnitude index
	FLargeNumber Result;
	Result.Value = Value * Other.Value;
	Result.MagnitudeIndex = MagnitudeIndex + Other.MagnitudeIndex;
	Result.Normalize();
	return Result;
}

FLargeNumber& FLargeNumber::operator*=(const FLargeNumber& Other)
{
	Value *= Other.Value;
	MagnitudeIndex += Other.MagnitudeIndex;
	Normalize();
	return *this;
}

FLargeNumber& FLargeNumber::operator++()    
{
	*this += FLargeNumber(1.0f, 0);
	return *this;
}

FLargeNumber FLargeNumber::operator++(int)
{
	FLargeNumber Temp = *this;
	++(*this);
	return Temp;
}


FString UMainWorldSubsystem::FormatLargeNumber(const FLargeNumber& Number) const
{
	FLargeNumber NormalizedNumber = Number;
	NormalizedNumber.Normalize();
	
	FString FormattedString;
	if (Number.MagnitudeIndex == 0)
	{
		FormattedString = FString::Printf(TEXT("%.2f"), NormalizedNumber.Value);
	}
	else
	{
		FormattedString = FString::Printf(TEXT("%.2f %s"), NormalizedNumber.Value, *NumberSuffixes[NormalizedNumber.MagnitudeIndex]);
	}
	return FormattedString;
}








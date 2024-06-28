#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MainWorldSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FLargeNumber
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Value;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MagnitudeIndex;

	FLargeNumber() : Value(0.0f), MagnitudeIndex(0) {}
	FLargeNumber(float InValue, int32 InMagnitudeIndex) : Value(InValue), MagnitudeIndex(InMagnitudeIndex) { Normalize(); }
	FLargeNumber(double InValue) : Value(InValue), MagnitudeIndex(0) { Normalize(); }

	void Normalize();

	FLargeNumber operator+(const FLargeNumber& Other) const;
	FLargeNumber operator-(const FLargeNumber& Other) const;
	FLargeNumber& operator+=(const FLargeNumber& Other);
	FLargeNumber& operator-=(const FLargeNumber& Other);

	bool operator>=(const FLargeNumber& Other) const;
	bool operator<=(const FLargeNumber& Other) const;
	bool operator>(const FLargeNumber& Other) const;
	bool operator<(const FLargeNumber& Other) const;
	bool operator==(const FLargeNumber& Other) const;
	bool operator!=(const FLargeNumber& Other) const;

	FLargeNumber operator*(float Multiplier) const;
	FLargeNumber& operator*=(float Multiplier);

	FLargeNumber operator*(const FLargeNumber& Other) const;
	FLargeNumber& operator*=(const FLargeNumber& Other);

	FLargeNumber& operator++();
	FLargeNumber operator++(int);
};

UCLASS()
class IDLEGAME_API UMainWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "LargeNumber")
	FString FormatLargeNumber(const FLargeNumber& Number) const;
};

static const TArray<FString> NumberSuffixes = {
	TEXT(""),
	TEXT("K"),
	TEXT("M"),
	TEXT("B"),
	TEXT("T"),
	TEXT("Qa"),
	TEXT("Qi"),
	TEXT("Sx"),
	TEXT("Sp"),
	TEXT("Oc"),
	TEXT("No"),
	TEXT("Dc"),
	TEXT("UDc"),
	TEXT("DDc"),
	TEXT("TDc"),
	TEXT("QaDc"),
	TEXT("QiDc"),
	TEXT("SxDc"),
	TEXT("SpDc"),
	TEXT("OcDc"),
	TEXT("NoDc"),
	TEXT("Vg"),
	TEXT("UVg"),
	TEXT("DVg"),
	TEXT("TVg"),
	TEXT("QaVg"),
	// Add more suffixes as needed
};

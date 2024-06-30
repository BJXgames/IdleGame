#pragma once

#include "CoreMinimal.h"
#include "MainPlayerController.h"
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
	TEXT("QiVg"),
	TEXT("SxVg"),
	TEXT("SpVg"),
	TEXT("OcVg"),
	TEXT("NoVg"),
	TEXT("Tg"),
	TEXT("UTg"),
	TEXT("DTg"),
	TEXT("TTg"),
	TEXT("QaTg"),
	TEXT("QiTg"),
	TEXT("SxTg"),
	TEXT("SpTg"),
	TEXT("OcTg"),
	TEXT("NoTg"),
	TEXT("Qd"),
	TEXT("UQd"),
	TEXT("DQd"),
	TEXT("TQd"), 
	TEXT("QaQd"),
	TEXT("QiQd"),
	TEXT("SxQd"),
	TEXT("SpQd"),
	TEXT("OcQd"),
	TEXT("NoQd"),
	TEXT("Qg"),
	TEXT("UQg"),
	TEXT("DQg"),
	TEXT("TQg"),
	TEXT("QaQg"),
	TEXT("QiQg"),
	TEXT("SxQg"),
	TEXT("SpQg"),
	TEXT("OcQg"),
	TEXT("NoQg"),
	TEXT("Sg"),
	TEXT("USg"),
	TEXT("DSg"),
	TEXT("TSg"),
	TEXT("QaSg"),
	TEXT("QiSg"),
	TEXT("SxSg"),
	TEXT("SpSg"),
	TEXT("OcSg"),
	TEXT("NoSg"),
	TEXT("St"),
	TEXT("USt"),
	TEXT("DSt"),
	TEXT("TSt"),
	TEXT("QaSt"),
	TEXT("QiSt"),
	TEXT("SxSt"),
	TEXT("SpSt"),
	TEXT("OcSt"),
	TEXT("NoSt"),
	TEXT("Og"),
	TEXT("UOg"),
	TEXT("DOg"),
	TEXT("TOg"),
	TEXT("QaOg"),
	TEXT("QiOg"),
	TEXT("SxOg"),
	TEXT("SpOg"),
	TEXT("OcOg"),
	TEXT("NoOg"),
	TEXT("Nn"),
	TEXT("UNn"),
	TEXT("DNn"),
	TEXT("TNn"),
	TEXT("QaNn"),
	TEXT("QiNn"),
	TEXT("SxNn"),
	TEXT("SpNn"),
	TEXT("OcNn"),
	TEXT("NoNn")
	// Add more suffixes as needed
};


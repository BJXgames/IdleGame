#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ManagerData.generated.h"

UENUM(BlueprintType)
enum class EManagerRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

USTRUCT(BlueprintType)
struct FManagerData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Manager")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Manager")
	float SpeedBoost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Manager")
	float IncomeMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Manager")
	float MoneyPriceReduction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Manager")
	EManagerRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Manager")
	UTexture2D* ManagerImage;

	FManagerData()
		: Name("Default"), SpeedBoost(1.1f), IncomeMultiplier(1.1f), MoneyPriceReduction(0.95f), Rarity(EManagerRarity::Common), ManagerImage(nullptr)
	{}
};

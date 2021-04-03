// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Element.generated.h"

UENUM(BlueprintType, Blueprintable)
enum EMagicElement
{
	ME_Any UMETA(DisplayName = "Any"),
    ME_Fire UMETA(DisplayName = "Fire"),
    ME_Water UMETA(DisplayName = "Water"),
    ME_Earth UMETA(DisplayName = "Earth"),
    ME_Wind UMETA(DisplayName = "Wind"),
    ME_Light UMETA(DisplayName = "Light"),
    ME_Dark UMETA(DisplayName = "Dark"),
    ME_Nature UMETA(DisplayName = "Nature"),
    ME_Lightning UMETA(DisplayName = "Lightning"),
    ME_Ice UMETA(DisplayName = "Ice"),
    ME_Void UMETA(DisplayName = "Void"),

    ME_MAX
};

USTRUCT(BlueprintType)
struct FElementPower
{
	GENERATED_BODY()

	FElementPower() {}
	explicit FElementPower(const TEnumAsByte<EMagicElement> SetElement, const float SetPower = 0.f)
	{
		Element = SetElement;
		Power = SetPower;
	}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<EMagicElement> Element = ME_Fire;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Power = 0.f;
};
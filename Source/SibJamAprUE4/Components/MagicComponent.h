// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SibJamAprUE4/Magic/BaseAbility.h"

#include "MagicComponent.generated.h"


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
struct FElementCombinations : public FTableRowBase
{
	GENERATED_BODY()
	
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TEnumAsByte<EMagicElement>> Elements = {};	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EMagicElement> FinalElement = ME_Any;
};


USTRUCT(BlueprintType)
struct FElementsGroup
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TEnumAsByte<EMagicElement>> Elements = {};
	
	bool operator== (const FElementsGroup RHS) const
	{
		int32 i = -1;
		for(auto& Element : RHS.Elements)
		{
			i++;
			if(Element != Elements[i])
				return false;
		}
		return true;
	}
};
USTRUCT(BlueprintType)
struct FAbilityCombinations : public FTableRowBase
{
	GENERATED_BODY()

	FAbilityCombinations() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FElementsGroup> ElementsGroups = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABaseAbility> AbilityClass;

	bool operator== (const FAbilityCombinations RHS) const
	{
		if (ElementsGroups == RHS.ElementsGroups)
		{
			return true;
		}
		return false;
	}
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

UCLASS(Blueprintable, BlueprintType)
class SIBJAMAPRUE4_API UMagicComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UMagicComponent();

	UFUNCTION(BlueprintCallable)
	void StartChargeMagic();
	UFUNCTION(BlueprintCallable)
	void ReleaseMagic();

	UFUNCTION(BlueprintCallable)
	void ChangeMagicElement(TEnumAsByte<EMagicElement> NewMagicElement);

protected:
	virtual void BeginPlay() override;

	TEnumAsByte<EMagicElement> CheckElementsCombinations(const TArray<TEnumAsByte<EMagicElement>> Elements) const;
	TSoftClassPtr<ABaseAbility> CheckAbilityCombinations();

	//Getters
	float GetMaxChargePower() const {return MaxChargePower;}
	float GetChargeMultiplication() const {return ChargeMultiplication;}
	
private:
	
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCharging = false;

	UPROPERTY(EditAnywhere, Category = "Charge")
	float ChargeMultiplication = 15.f;
	UPROPERTY(EditAnywhere, Category = "Charge")
	float MaxChargePower = 100.f;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Ability")
	TSoftClassPtr<ABaseAbility> CurrentAbilityClass = nullptr;
	UPROPERTY(VisibleInstanceOnly, Category = "Ability")
	TArray<FAbilityCombinations> AvailableAbilityCombinations = {};
	
	void Charging();
	

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Elements", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EMagicElement> CurrentMagicElement = ME_Fire;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Elements", meta = (AllowPrivateAccess = "true"))
	TArray<FElementPower> ElementsPowers = {};

	float GetFullElementsPower();
	
	FTimerHandle ChargingTimer;
	
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* ElementsDataTable = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	UDataTable* AbilitiesDataTable = nullptr;

	void FindElementReceipt(TEnumAsByte<EMagicElement> Element, TArray<TEnumAsByte<EMagicElement>>& Elements) const;
};

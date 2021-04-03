// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicComponent.h"

UMagicComponent::UMagicComponent()
{
}

void UMagicComponent::BeginPlay()
{
	Super::BeginPlay();
}


TEnumAsByte<EMagicElement> UMagicComponent::CheckElementsCombinations(
	const TArray<TEnumAsByte<EMagicElement>> Elements) const
{
	TArray<FName> Names = ElementsDataTable->GetRowNames();
	for (auto& Name : Names)
	{
		bool bNotAcceptable = false;
		const FElementCombinations Combination = *ElementsDataTable->FindRow<FElementCombinations>(Name, "");
		TArray<TEnumAsByte<EMagicElement>> LocalElements = Combination.Elements;

		for (auto& Elem : Elements)
		{
			if (LocalElements.Find(Elem) == INDEX_NONE)
			{
				bNotAcceptable = true;
				break;
			}
			LocalElements.RemoveAt(LocalElements.Find(Elem));
		}

		if (bNotAcceptable)
			continue;

		return Combination.FinalElement;
	}
	return ME_Any;
}


TSoftClassPtr<ABaseAbility> UMagicComponent::CheckAbilityCombinations()
{
	const int CurrentIndex = ElementsPowers.Num() - 1;
	TArray<FAbilityCombinations> AbilitiesToRemove = {};
	
	for (const auto& Ability : AvailableAbilityCombinations)
	{
		if(Ability.ElementsGroups.Num() <= CurrentIndex)
		{
			UE_LOG(LogTemp, Display, TEXT("Delete by range"));
			UE_LOG(LogTemp, Display, TEXT("------------------------------------"));

			
			AbilitiesToRemove.Add(Ability);
			continue;
		}
		
		bool bAvailable = false;
		for (const auto& Element : Ability.ElementsGroups[CurrentIndex].Elements)
		{
			UE_LOG(LogTemp, Display, TEXT("Element: %s"), *UEnum::GetValueAsString(Element));
			if (Element != ME_Fire && Element != ME_Water && Element != ME_Earth && Element != ME_Wind && Element !=
                ME_Light && Element != ME_Dark)
			{
				TArray<TEnumAsByte<EMagicElement>> BaseElements = {};
				FindElementReceipt(Element, BaseElements);
				for(const auto& Element123 : BaseElements)
				{
					UE_LOG(LogTemp, Display, TEXT("  SubElement - %s"), *UEnum::GetValueAsString(Element123));
				}
				
				if(BaseElements.Find(CurrentMagicElement) != INDEX_NONE)
				{
					UE_LOG(LogTemp, Display, TEXT("Find as part of hard element"));
					bAvailable = true;
					break;
				}
			}
			else
			{
				if(Element == CurrentMagicElement)
				{
					UE_LOG(LogTemp, Display, TEXT("Find as simple element"));
					bAvailable = true;
					break;
				}
			}
		}
		
		if(!bAvailable)
		{
			UE_LOG(LogTemp, Display, TEXT("Remove"));
			AbilitiesToRemove.Add(Ability);
		}
		else
			UE_LOG(LogTemp, Display, TEXT("Retain"));

		UE_LOG(LogTemp, Display, TEXT("------------------------------------"));
	}

	for(const auto& AbilityToRemove : AbilitiesToRemove)
		AvailableAbilityCombinations.Remove(AbilityToRemove);

	
	UE_LOG(LogTemp, Display, TEXT("------------------------------------"));
	UE_LOG(LogTemp, Display, TEXT("------------------------------------"));
	UE_LOG(LogTemp, Display, TEXT("------------------------------------"));

	
	return {};
}


float UMagicComponent::GetFullElementsPower()
{
	float FullPower = 0.f;
	for (auto& ElementPower : ElementsPowers)
		FullPower += ElementPower.Power;

	return FullPower;
}

void UMagicComponent::FindElementReceipt(const TEnumAsByte<EMagicElement> Element,
                                         TArray<TEnumAsByte<EMagicElement>>& Elements) const
{
	TArray<FName> Names = ElementsDataTable->GetRowNames();
	for (const auto& Name : Names)
	{
		const FElementCombinations Combination = *ElementsDataTable->FindRow<FElementCombinations>(Name, "");

		if (Combination.FinalElement == Element)
			Elements = Combination.Elements;
	}
}


void UMagicComponent::StartChargeMagic()
{
	if (bCharging) return;
	bCharging = true;


	ElementsPowers.Empty();
	ElementsPowers.Add(FElementPower(CurrentMagicElement));

	AvailableAbilityCombinations.Empty();
	
	TArray<FName> Names = AbilitiesDataTable->GetRowNames();
	for (auto& Name : Names)
	{
		FAbilityCombinations Combination = *AbilitiesDataTable->FindRow<FAbilityCombinations>(Name, "");

		if (Combination.ElementsGroups.Num() == 1)
			if (Combination.ElementsGroups[0].Elements.Find(CurrentMagicElement) != INDEX_NONE)
				CurrentAbilityClass = Combination.AbilityClass;

		AvailableAbilityCombinations.Add(Combination);
	}

	CheckAbilityCombinations();


	GetWorld()->GetTimerManager().SetTimer(ChargingTimer, this, &UMagicComponent::Charging, 0.1, true);
}


void UMagicComponent::Charging()
{
	if (GetFullElementsPower() >= 100.f) return;

	const float ChargeTime = GetWorld()->GetTimerManager().GetTimerElapsed(ChargingTimer);
	const float ChargeValue = ChargeTime * GetChargeMultiplication();

	ElementsPowers.Last().Power += FMath::Clamp(ChargeValue, 0.f, GetMaxChargePower() - GetFullElementsPower());
}


void UMagicComponent::ReleaseMagic()
{
	if (!bCharging) return;
	bCharging = false;


	GetWorld()->GetTimerManager().ClearTimer(ChargingTimer);
}


void UMagicComponent::ChangeMagicElement(const TEnumAsByte<EMagicElement> NewMagicElement)
{
	CurrentMagicElement = NewMagicElement;

	if (!bCharging || (GetMaxChargePower() - GetFullElementsPower() < 5.f)) return;
	//Continue if the magic is charging now

	const TEnumAsByte<EMagicElement> PreviousElement = ElementsPowers.Last().Element;

	const TEnumAsByte<EMagicElement> CombineElement = CheckElementsCombinations(
		TArray<TEnumAsByte<EMagicElement>>({PreviousElement, CurrentMagicElement}));

	if (CombineElement != ME_Any)
	{
		CurrentMagicElement = CombineElement;
		ElementsPowers.Last().Element = CombineElement;
	}
	else
		ElementsPowers.Add(FElementPower(CurrentMagicElement));

	CheckAbilityCombinations();

	if (AvailableAbilityCombinations.Num() <= 0)
	{
		CurrentAbilityClass = nullptr;
		ReleaseMagic();
	}
}

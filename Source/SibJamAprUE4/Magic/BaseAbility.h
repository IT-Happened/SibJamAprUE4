// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SibJamAprUE4/Structures/Element.h"

#include "BaseAbility.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class SIBJAMAPRUE4_API ABaseAbility : public AActor
{
	GENERATED_BODY()

public:
	ABaseAbility();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Ability", meta = (DisplayName = "UseAbility"))
	void BP_UseAbility(const TArray<FElementPower>& Elements);
	
protected:
	virtual void BeginPlay() override;

private:
	
};

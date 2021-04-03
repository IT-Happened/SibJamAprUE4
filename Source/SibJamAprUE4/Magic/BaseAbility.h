// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAbility.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class SIBJAMAPRUE4_API ABaseAbility : public AActor
{
	GENERATED_BODY()

public:
	ABaseAbility();

protected:
	virtual void BeginPlay() override;

};

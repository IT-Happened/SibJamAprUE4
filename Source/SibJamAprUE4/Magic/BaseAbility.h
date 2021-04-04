// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SibJamAprUE4/Structures/Element.h"

#include "BaseAbility.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndAbility);

UCLASS(Abstract, Blueprintable, BlueprintType)
class SIBJAMAPRUE4_API ABaseAbility : public AActor
{
	GENERATED_BODY()

public:
	ABaseAbility();

	virtual void UseAbility(const TArray<FElementPower>& AbilityElements);

	FOnEndAbility EndAbilityDelegate;

protected:
	virtual void BeginPlay() override;
	
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "UseAbility"))
	void BP_UseAbility(const TArray<FElementPower>& AbilityElements);

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	float Damage = 5.f;
	

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float ReadyTime = 0.35f;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	float ReturnTime = 0.4f;

	
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* AttackMontage = nullptr;

	
	TArray<FElementPower> Elements = {};

	
	void WaitAbility();
	UFUNCTION(BlueprintCallable)
	void EndAbility();

private:
	
};

UCLASS(Abstract)
class ASprayAbility : public ABaseAbility
{
	GENERATED_BODY()

public:
	ASprayAbility();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	float Duration = 1.f;


private:

	
};
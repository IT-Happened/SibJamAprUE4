// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbility.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


ABaseAbility::ABaseAbility() {}

void ABaseAbility::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAbility::UseAbility(const TArray<FElementPower>& AbilityElements)
{
	Elements = AbilityElements;
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	const float SectionTime = AttackMontage->GetSectionLength(0);

	const float PlayRate = SectionTime / ReadyTime;

	OwnerCharacter->PlayAnimMontage(AttackMontage, PlayRate);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_None);

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ABaseAbility::WaitAbility, ReadyTime);
}

void ABaseAbility::WaitAbility()
{
	Cast<ACharacter>(GetOwner())->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	BP_UseAbility(Elements);	
}

void ABaseAbility::EndAbility()
{	
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	if(!OwnerCharacter) return;
	
	OwnerCharacter->StopAnimMontage();
	
	const uint8 LastIndex = AttackMontage->CompositeSections.Num() - 1;
	const FCompositeSection& LastSection = AttackMontage->CompositeSections.Last();
	
	const float SectionTime = AttackMontage->GetSectionLength(LastIndex);

	const float PlayRate = SectionTime / ReturnTime;

	OwnerCharacter->PlayAnimMontage(AttackMontage, PlayRate, LastSection.SectionName);

	EndAbilityDelegate.Broadcast();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

ASprayAbility::ASprayAbility() {}

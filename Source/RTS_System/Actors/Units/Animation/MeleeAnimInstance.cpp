// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAnimInstance.h"
#include "../BaseMeleeUnit.h"


UMeleeAnimInstance::UMeleeAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_BasicAttack(
		TEXT("/Game/Mannequin/Animations/AM_BasicAttack.AM_BasicAttack"));

	if (MON_BasicAttack.Succeeded())
		BasicAttackMontage = MON_BasicAttack.Object;
}

void UMeleeAnimInstance::PlayBasicAttack() {
	Montage_Play(BasicAttackMontage, 1.5f);
}

void UMeleeAnimInstance::AnimNotify_AttackHit() {
	auto IUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	if (IsValid(IUnit)) 
		IUnit->AttackCheck();
}

void UMeleeAnimInstance::AnimNotify_AttackEnd() {
	auto IUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	if (IsValid(IUnit)) 
		IUnit->TurnOffBehavior(UNIT_BEHAVIOR::ATTACKING);
}

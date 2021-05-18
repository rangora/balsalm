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
	auto IUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	IUnit->TurnOffBehavior(UNIT_BEHAVIOR::MOVABLE);
	IUnit->StopMovement();
	Montage_Play(BasicAttackMontage, 1.8f);
}

void UMeleeAnimInstance::AnimNotify_AttackHit() {
	auto IUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	if (IsValid(IUnit)) 
		IUnit->AttackCheck();
}

void UMeleeAnimInstance::AnimNotify_AttackEnd() {
	auto IUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	if (IsValid(IUnit)) {
		IUnit->TurnOnBehavior(UNIT_BEHAVIOR::MOVABLE);
		IUnit->TurnOffBehavior(UNIT_BEHAVIOR::ATTACKING);
	}
}

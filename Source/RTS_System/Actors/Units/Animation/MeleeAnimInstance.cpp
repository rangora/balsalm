// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAnimInstance.h"
#include "../Unit.h"


UMeleeAnimInstance::UMeleeAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_BasicAttack(
		TEXT("/Game/Mannequin/Animations/AM_BasicAttack.AM_BasicAttack"));

	if (MON_BasicAttack.Succeeded())
		BasicAttackMontage = MON_BasicAttack.Object;
}

void UMeleeAnimInstance::PlayBasicAttack() {
	Montage_Play(BasicAttackMontage, 1.f);
}

void UMeleeAnimInstance::AnimNotify_AttackHit() {
	auto IUnit = Cast<AUnit>(GetOwningActor());
	if (!IsValid(IUnit)) return;
	if (DeleFunc_AttackCheck.IsBound())
		DeleFunc_AttackCheck.Broadcast();
}

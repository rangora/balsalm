// Fill out your copyright notice in the Description page of Project Settings.


#include "AxeSkillAnimInstance.h"
#include "MeleeAnimInstance.h"
#include "../BaseMeleeUnit.h"
#include "../Equipment/BaseWeapon.h"
#include "Engine/Engine.h"


UAxeSkillAnimInstance::UAxeSkillAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SkullCrack(
		TEXT("/Game/Mannequin/Animations/AM_Axe_SkullCrack.AM_Axe_SkullCrack"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_CycloneAxe(
		TEXT("/Game/Mannequin/Animations/AM_Axe_CycloneAxe.AM_Axe_CycloneAxe"));

	SkillCrack_Hit = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffect"));


	if (MON_SkullCrack.Succeeded()) 
		SkullCrackMontage = MON_SkullCrack.Object;
	if (MON_CycloneAxe.Succeeded())
		CycloneAxeMontage = MON_CycloneAxe.Object;

	SkillCrack_Hit->bAutoActivate = false;
	SkillCrack_Hit->SetCollisionProfileName("NoCollision");
}

void UAxeSkillAnimInstance::PlaySkullCrack() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	aUnit->TurnOffBehavior(UNIT_BEHAVIOR::MOVABLE);
	Montage_Play(SkullCrackMontage, 1.7f);
}

void UAxeSkillAnimInstance::PlayCycloneAxe() {
	Montage_Play(CycloneAxeMontage, 1.f);
}

void UAxeSkillAnimInstance::AnimNotify_SkillEnd() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());

	if (IsValid(aUnit)) {
		aUnit->TargetUnit = nullptr;
		aUnit->bGoBasicAnimInstance = true;
		aUnit->TurnOffBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE);
		aUnit->TurnOnBehavior(UNIT_BEHAVIOR::MOVABLE);
	}
}

void UAxeSkillAnimInstance::AnimNotify_SkullCrackHit() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	if (IsValid(aUnit)) {
		aUnit->Weapon->ActiveHitParticle(SkillCrack_Hit);
		aUnit->SkillAttackCheck();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AxeSkillAnimInstance.h"
#include "MeleeAnimInstance.h"
#include "../BaseMeleeUnit.h"


UAxeSkillAnimInstance::UAxeSkillAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SkullCrack(
		TEXT("/Game/Mannequin/Animations/AM_Axe_SkullCrack.AM_Axe_SkullCrack"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_CycloneAxe(
		TEXT("/Game/Mannequin/Animations/AM_Axe_CycloneAxe.AM_Axe_CycloneAxe"));

	if (MON_SkullCrack.Succeeded()) 
		SkullCrackMontage = MON_SkullCrack.Object;
	if (MON_CycloneAxe.Succeeded())
		CycloneAxeMontage = MON_CycloneAxe.Object;

}

void UAxeSkillAnimInstance::PlaySkullCrack() {
	Montage_Play(SkullCrackMontage, 1.f);
}

void UAxeSkillAnimInstance::PlayCycloneAxe() {
	Montage_Play(CycloneAxeMontage, 1.f);
}

void UAxeSkillAnimInstance::AnimNotify_SkillEnd() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());

	if (IsValid(aUnit)) 
		aUnit->bGoBasicAnimInstance = true;
}

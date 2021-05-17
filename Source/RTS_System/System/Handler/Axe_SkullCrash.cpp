// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe_SkullCrash.h"
#include "CoolDownHandler.h"
#include "../../Actors/Units/Animation/AxeSkillAnimInstance.h"
#include "../../Actors/Units/BaseMeleeUnit.h"
#include "../../DataTable/ABaseSkillTable.h"


UAxe_SkullCrash::UAxe_SkullCrash() {
	Skill_ID = "1";

	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_AxeSkill(
		TEXT("/Game/Mannequin/Animations/ANIM_UnitAxeSkill.ANIM_UnitAxeSkill_C"));

	if (ANIM_AxeSkill.Succeeded())
		AxeSkillAnimInstanceClass = ANIM_AxeSkill.Class;
}

void UAxe_SkullCrash::CheckSkillRange(AUnit* pUnit) {
	float skillBasicAttack_amount = SkillParams->Variable01;
	float skillAttack_rate = SkillParams->Variable02;
	float skill_range = SkillParams->Variable03;

	auto IUnit = Cast<ABaseMeleeUnit>(pUnit);

	IUnit->AppointTheSkillTarget(skill_range);
}

void UAxe_SkullCrash::ActiveSkill(AUnit* pUnit) {
	auto IUnit = Cast<ABaseMeleeUnit>(pUnit);

	// anim change
	if (IsValid(IUnit)) {
		CheckSkillRange(IUnit);
	}
}


void UAxe_SkullCrash::PlaySkillAnimation(AUnit* pUnit) {
	auto IUnit = Cast<ABaseMeleeUnit>(pUnit);

	IUnit->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	IUnit->GetMesh()->SetAnimInstanceClass(AxeSkillAnimInstanceClass);

	auto pAnim = IUnit->GetMesh()->GetAnimInstance();
	auto AxeAnim = Cast<UAxeSkillAnimInstance>(pAnim);

	if (IsValid(AxeAnim)) {
		AxeAnim->PlaySkullCrack();
	}
}

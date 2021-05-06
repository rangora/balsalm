// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe_SkullCrash.h"
#include "../../Actors/Units/Animation/AxeSkillAnimInstance.h"
#include "../../Actors/Units/BaseMeleeUnit.h"


UAxe_SkullCrash::UAxe_SkullCrash() {
	Skill_ID = "1";

	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_AxeSkill(
		TEXT("/Game/Mannequin/Animations/ANIM_UnitAxeSkill.ANIM_UnitAxeSkill_C"));

	if (ANIM_AxeSkill.Succeeded())
		AxeSkillAnimInstanceClass = ANIM_AxeSkill.Class;
}

void UAxe_SkullCrash::ActiveSkill(AUnit* pUnit) {
	auto PlayerUnit = Cast<ABaseMeleeUnit>(pUnit);

	// anim change
	if (IsValid(PlayerUnit)) {
		PlayerUnit->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		PlayerUnit->GetMesh()->SetAnimInstanceClass(AxeSkillAnimInstanceClass);

		auto pAnim = PlayerUnit->GetMesh()->GetAnimInstance();
		auto AxeAnim = Cast<UAxeSkillAnimInstance>(pAnim);

		if (IsValid(AxeAnim)) {
			GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Green, TEXT("UAxe_SkullCrash!!"));
			AxeAnim->PlaySkullCrack();
		}
	}
}

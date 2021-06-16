// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword_DoubleStrike.h"
#include "../../UI/MainHUD.h"
#include "../../Actors/Units/BaseMeleeUnit.h"
#include "../../DataTable/ABaseSkillTable.h"
#include "../../Actors/Units/Animation/SwordSkillAnimInstance.h"




USword_DoubleStrike::USword_DoubleStrike() {
	Skill_ID = "5";

	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_SwordSkill(
		TEXT("/Game/Mannequin/Animations/ANIM_UnitSwordSkill.ANIM_UnitSwordSkill_C"));

	if (ANIM_SwordSkill.Succeeded())
		SkillAnimInstanceClass = ANIM_SwordSkill.Class;
}

void USword_DoubleStrike::ActiveSkill(AUnit* pUnit) {
	auto IUnit = Cast<ABaseMeleeUnit>(pUnit);

	if (IsValid(IUnit)) 
		CheckSkillRange(IUnit);
}

void USword_DoubleStrike::CheckSkillRange(AUnit* pUnit) {
	auto IUnit = Cast<ABaseMeleeUnit>(pUnit);
	auto IController = UGameplayStatics::GetPlayerController(pUnit->GetWorld(), 0);
	auto IHUD = Cast<AMainHUD>(IController->GetHUD());

	float skill_range = SkillParams->Variable03;

	// Set skill decal size.
	FVector NewSkillSize = IUnit->DecalSkillRange->DecalSize;
	NewSkillSize.Y = skill_range;
	NewSkillSize.Z = skill_range;
	IUnit->DecalSkillRange->DecalSize = NewSkillSize;
	IUnit->skillRadius = skill_range;

	// Show skill radius.
	IUnit->DecalSkillRange->SetVisibility(true);
	IUnit->TurnOnBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);

	IHUD->SetMouseLeftButtonAction(LeftButtonAction::TARGETING);
}

void USword_DoubleStrike::PlaySkillAnimation(AUnit* pUnit) {
	auto IUnit = Cast<ABaseMeleeUnit>(pUnit);

	IUnit->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	IUnit->GetMesh()->SetAnimInstanceClass(SkillAnimInstanceClass);

	auto pAnim = IUnit->GetMesh()->GetAnimInstance();
	auto SwordAnim = Cast<USwordSkillAnimInstance>(pAnim);

	if (IsValid(SwordAnim)) {
		SwordAnim->PlayDoubleStrike();
	}
}

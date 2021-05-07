// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillObject.h"
#include "../../DataTable/ABaseSkillTable.h"
#include "Engine/Engine.h"



USkillObject::USkillObject() {
	Skill_ID = EMPTY_SKILL_ID;
	SkillParams = new SkillVariable;
}

void USkillObject::ActiveSkill(AUnit* pUnit) {}

void USkillObject::SkillAction(ABaseMeleeUnit* pUnit) {}

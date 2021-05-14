// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillAnimHandler.h"
#include "../../ActorType.h"
#include "../../DataTable/ABaseSkillTable.h"



USkillAnimHandler::USkillAnimHandler() {
	Skill_ID = EMPTY_SKILL_ID;
}

void USkillAnimHandler::CheckSkillRange(AUnit* pUnit) {}

void USkillAnimHandler::ActiveSkill(AUnit* pUnit) {}

void USkillAnimHandler::SkillAction(AUnit* pUnit) {}

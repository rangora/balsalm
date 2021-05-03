// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillObject.h"
#include "../../DataTable/ABaseSkillTable.h"


USkillObject::USkillObject() {
	Skill_ID = EMPTY_SKILL_ID;

	SkillParams = new SkillVariable;
}

void USkillObject::ActiveSkill() {}

void USkillObject::Clone(const USkillObject* From) {
	Skill_ID = From->Skill_ID;
	SkillParams->Skill_ID = From->SkillParams->Skill_ID;
	SkillParams->Variable01 = From->SkillParams->Variable01;
	SkillParams->Variable02 = From->SkillParams->Variable02;
	SkillParams->Variable03 = From->SkillParams->Variable03;
	SkillParams->Variable04 = From->SkillParams->Variable04;
	SkillParams->Variable05 = From->SkillParams->Variable05;
	SkillParams->WeaponType = From->SkillParams->WeaponType;
	SkillParams->SkillType = From->SkillParams->SkillType;
	SkillParams->SkillName = From->SkillParams->SkillName;
	SkillParams->Description = From->SkillParams->Description;
	SkillParams->ThumbnailTexture = From->SkillParams->ThumbnailTexture;
}

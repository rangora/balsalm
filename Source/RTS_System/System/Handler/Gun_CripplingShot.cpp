// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun_CripplingShot.h"

UGun_CripplingShot::UGun_CripplingShot() {
	Skill_ID = "3";
	WeaponType = WEAPONTYPE::GUN;
	SkillType = SKILLTYPE::TARGETINGSKILL;
	SkillName = "CripplingShot";
	CoolTime = 5.f;
	Description = "Crippp";
}

void UGun_CripplingShot::ActiveSkill() {
}

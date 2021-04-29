// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe_DualStrike.h"

UAxe_DualStrike::UAxe_DualStrike() {
	Skill_ID = "2";
	WeaponType = WEAPONTYPE::AXE;
	SkillType = SKILLTYPE::TARGETINGSKILL;
	SkillName = "DualStrike";
	CoolTime = 6.f;
	Description = "DUAL!!";
}

void UAxe_DualStrike::ActiveSkill() {
}

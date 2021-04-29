// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe_SkullCrash.h"
#include "Engine/Texture2D.h"

UAxe_SkullCrash::UAxe_SkullCrash() {
	Skill_ID = "1";
	WeaponType = WEAPONTYPE::AXE;
	SkillType = SKILLTYPE::TARGETINGSKILL;
	SkillName = "SkullCrash";
	CoolTime = 4.f;
	Description = "Baaaak";
}

void UAxe_SkullCrash::ActiveSkill() {
}

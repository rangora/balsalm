// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCaculator.h"
#include "../Units/BaseMeleeUnit.h"
#include "../Units/Component/StatComponent.h"
#include "../Units/Equipment/BaseWeapon.h"
#include "../../System/Handler/SkillAnimHandler.h"
#include "../../DataTable/ABaseSkillTable.h"



float UAttackCaculator::CaculateDamage(const AUnit* From, const AUnit* To) {
	float amount = From->UnitStat->currentDamage;

	switch (From->Weapon->AttackType) {
	case ATTACKTYPE::CUT: {
		break;
	}
	case ATTACKTYPE::CRUSH: {
		amount -= To->UnitStat->Armor * 1.0f;
		break;
	}
	case ATTACKTYPE::PIERCE: {
		break;
	}
	}

	return amount;
}

float UAttackCaculator::SkillDamage(const AUnit* From, const AUnit* To) {
	auto Attacker = Cast<ABaseMeleeUnit>(From);
	
	auto SkillParamsRef = Attacker->SkillRef->SkillParams;
	float amount = SkillParamsRef->Variable01;
	amount += SkillParamsRef->Variable02 * Attacker->UnitStat->currentDamage;
	amount -= To->UnitStat->Armor;

	return amount;
}

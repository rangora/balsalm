// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCaculator.h"
#include "../Units/Unit.h"
#include "../Units/Component/StatComponent.h"
#include "../Units/Equipment/BaseWeapon.h"
//#include "ActorType.h"
//#include "../../ActorType.h"

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
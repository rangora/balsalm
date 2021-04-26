// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataHandler.h"
#include "Engine.h"
#include "Components/Widget.h"
#include "../../UI/WeaponSkillSlot.h"
#include "../../DataTable/ABaseSkillTable.h"
#include "UObject/ConstructorHelpers.h"

USkillDataHandler::USkillDataHandler() {
	static ConstructorHelpers::FObjectFinder<UDataTable> AttackSkillTable_Class(
		TEXT("/Game/DataTable/Skill/AttackSkillTable.AttackSkillTable"));
	if (AttackSkillTable_Class.Succeeded()) {
		AttackSkillTable = AttackSkillTable_Class.Object;
	}

	AxeSkillIndex.Add(1);
	AxeSkillIndex.Add(3);
	GunSkillIndex.Add(2);
}

void USkillDataHandler::SetSkillData(SKILLTYPE skillType, int32 skill_id, BaseSkillData_Info* pSkillContainer) {
	switch (skillType) {
	
	case SKILLTYPE::WEAPON_MELEE: {
		auto skillContainer = (AttackSkillData_Info*)pSkillContainer;
		SetAttackSkillData(skill_id, skillContainer);
		break;
	}
	
	case SKILLTYPE::WEAPON_RANGE: {
		break;
	}
	case SKILLTYPE::UTILITY: {
		break;
	}
	}
}

void USkillDataHandler::SetAxeSkillData(TArray<UWidget*>& Slots) {
	for (int idx = 0; idx < Slots.Num(); idx++) {
		auto slot = Cast<UWeaponSkillSlot>(Slots[idx]);

		if (AxeSkillIndex.IsValidIndex(idx)) {
			slot->SkillData = new AttackSkillData_Info;
			SetSkillData(SKILLTYPE::WEAPON_MELEE, AxeSkillIndex[idx], slot->SkillData);
			slot->SetThumbnailImage();
			slot->bAssigned = true;
		}
		else {
			if (slot->SkillData != nullptr)
				delete slot->SkillData;
			slot->SetDefaultThumbnailImage();
			slot->bAssigned = false;
		}
	}
}

void USkillDataHandler::SetGunSkillData(TArray<UWidget*>& Slots) {
	for (int idx = 0; idx < Slots.Num(); idx++) {
		auto slot = Cast<UWeaponSkillSlot>(Slots[idx]);
		
		if (GunSkillIndex.IsValidIndex(idx)) {
			slot->SkillData = new AttackSkillData_Info;
			SetSkillData(SKILLTYPE::WEAPON_MELEE, GunSkillIndex[idx], slot->SkillData);
			slot->SetThumbnailImage();
		}
		else {
			if (slot->SkillData != nullptr)
				delete slot->SkillData;
			slot->SetDefaultThumbnailImage();
		}
	}
}

void USkillDataHandler::SetAttackSkillData(int32 skill_id, AttackSkillData_Info* pSkillContainer) {
	auto Skill_info = AttackSkillTable->FindRow<FAttackSkillDataTable>(*FString::FromInt(skill_id), TEXT(""));
	if (Skill_info != nullptr) {
		pSkillContainer->AttackRange = Skill_info->AttackRange;
		pSkillContainer->BaseDamage = Skill_info->BaseDamage;
		pSkillContainer->CoolTime = Skill_info->CoolTime;
		pSkillContainer->DamageRate = Skill_info->DamageRate;
		pSkillContainer->Description = Skill_info->Description;
		pSkillContainer->SkillName = Skill_info->SkillName;
		pSkillContainer->SkillType = Skill_info->SkillType;
		pSkillContainer->Skill_ID = Skill_info->Skill_ID;
		pSkillContainer->Thumbnail = Skill_info->Thumbnail;
	}
}

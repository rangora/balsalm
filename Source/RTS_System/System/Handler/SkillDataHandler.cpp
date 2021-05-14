// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillDataHandler.h"
#include "Engine.h"
#include "SkillAnimHandler.h"
#include "AllSkill.h"
#include "Components/Widget.h"
#include "UObject/ConstructorHelpers.h"
#include "../../UI/WeaponSkillSlot.h"
#include "../../DataTable/ABaseSkillTable.h"


USkillDataHandler::USkillDataHandler() {
	ConstructorHelpers::FObjectFinder<UDataTable> SkillVariableTable_Class(
		TEXT("/Game/DataTable/Skill/SkillVariableTable.SkillVariableTable"));
	
	if (SkillVariableTable_Class.Succeeded()) 
		SkillVariableTable = SkillVariableTable_Class.Object;
	

	SkillTableCreate();
	InitSkillVariable();
}

void USkillDataHandler::UpdateWeaponSkillSlot(TArray<UWidget*>& Slots, const WEAPONTYPE WeaponType) {
	SArray* Target = nullptr;

	switch (WeaponType) {
	case WEAPONTYPE::AXE: {
		Target = &AxeSkills;
		break;
	}
	case WEAPONTYPE::GUN: {
		Target = &GunSkills;
		break;
	}
	}
	SetWeaponSkilData(Slots, Target, WeaponType);
}

void USkillDataHandler::SetWeaponSkilData(TArray<UWidget*>& Slots, const SArray* SkillArray, WEAPONTYPE WeaponType) {
	if (SkillArray == nullptr) return;
	
	for (int idx = 0; idx < Slots.Num(); idx++) {
		auto slot = Cast<UWeaponSkillSlot>(Slots[idx]);

		if (IsValid(slot)) {
			if (SkillArray->IsValidIndex(idx)) {
				slot->CurrentTexture = (*SkillArray)[idx]->SkillAnimMgr->SkillParams->ThumbnailTexture;
				slot->SetThumbnailImage();
				slot->SkillObject = (*SkillArray)[idx];
				slot->bAssigned = true;
				slot->bDragable = true;
			}

			else {
				slot->SetDefaultThumbnailImage();
				slot->bAssigned = false;
				slot->bDragable = false;
			}
		}
	}
}


///// Private functions. /////

void USkillDataHandler::SkillTableCreate() {
	TArray<USkillAnimHandler*> SkillAnimArray;
	SkillAnimArray.Add(NewObject<UAxe_SkullCrash>());
	SkillAnimArray.Add(NewObject<UAxe_DualStrike>());
	
	for (int i = 0; i < SkillAnimArray.Num(); i++) {
		AxeSkills.Add(NewObject<USkillObject>());
		AxeSkills[i]->SkillAnimMgr = SkillAnimArray[i];
	}
}

void USkillDataHandler::InitSkillVariable() {
	TArray<SArray*> SkillIndexMatrix;
	SkillIndexMatrix.Add(&AxeSkills);
	//SkillIndexMatrix.Add(&GunSkills);

	for (auto Row : SkillIndexMatrix) {
		for (auto Element : *Row) {
			auto Origin = SkillVariableTable->FindRow<FSkillVariable>(Element->SkillAnimMgr->Skill_ID, "");
			Element->SkillAnimMgr->SkillParams = new SkillVariable;
			Element->SkillAnimMgr->SkillParams->Skill_ID = Origin->Skill_ID;
			Element->SkillAnimMgr->SkillParams->Variable01 = Origin->Variable01;
			Element->SkillAnimMgr->SkillParams->Variable02 = Origin->Variable02;
			Element->SkillAnimMgr->SkillParams->Variable03 = Origin->Variable03;
			Element->SkillAnimMgr->SkillParams->Variable04 = Origin->Variable04;
			Element->SkillAnimMgr->SkillParams->Variable05 = Origin->Variable05;
			Element->SkillAnimMgr->SkillParams->WeaponType = Origin->WeaponType;
			Element->SkillAnimMgr->SkillParams->SkillType = Origin->SkillType;
			Element->SkillAnimMgr->SkillParams->SkillName = Origin->SkillName;
			Element->SkillAnimMgr->SkillParams->Description = Origin->Description;
			Element->SkillAnimMgr->SkillParams->ThumbnailTexture = Origin->ThumbnailTexture;
		}
	}
}


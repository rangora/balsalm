// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillDataHandler.h"
#include "SkillAnimHandler.h"
#include "CoolDownHandler.h"
#include "SkillObject.h"
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
				slot->CurrentTexture = (*SkillArray)[idx]->GetThumbnailTexture();
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
	SkillAnimArray.Add(NewObject<UAxe_CycloneAxe>());
	
	for (int i = 0; i < SkillAnimArray.Num(); i++) {
		AxeSkills.Add(NewObject<USkillObject>());
		AxeSkills[i]->SetSkillAnimMgr(SkillAnimArray[i]);
	}
}

void USkillDataHandler::InitSkillVariable() {
	TArray<SArray*> SkillIndexMatrix;
	SkillIndexMatrix.Add(&AxeSkills);
	//SkillIndexMatrix.Add(&GunSkills);

	for (auto Row : SkillIndexMatrix) {
		for (auto Element : *Row) {
			auto Origin = SkillVariableTable->FindRow<FSkillVariable>(Element->GetID(), "");
			auto NewSkillVariable = new SkillVariable;
			NewSkillVariable->Skill_ID = Origin->Skill_ID;
			NewSkillVariable->Variable01 = Origin->Variable01;
			NewSkillVariable->Variable02 = Origin->Variable02;
			NewSkillVariable->Variable03 = Origin->Variable03;
			NewSkillVariable->Variable04 = Origin->Variable04;
			NewSkillVariable->Variable05 = Origin->Variable05;
			NewSkillVariable->WeaponType = Origin->WeaponType;
			NewSkillVariable->SkillType = Origin->SkillType;
			NewSkillVariable->SkillName = Origin->SkillName;
			NewSkillVariable->Description = Origin->Description;
			NewSkillVariable->ThumbnailTexture = Origin->ThumbnailTexture;
			Element->SetSkillParams(NewSkillVariable);
		}
	}
}


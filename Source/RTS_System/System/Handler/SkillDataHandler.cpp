// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataHandler.h"
#include "Engine.h"
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
	
	// Creating Skill Table..
	AxeSkills.Add(NewObject<UAxe_SkullCrash>());
	AxeSkills.Add(NewObject<UAxe_DualStrike>());
	GunSkills.Add(NewObject<UGun_CripplingShot>());

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
				slot->CurrentTexture = (*SkillArray)[idx]->SkillParams->ThumbnailTexture;
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

void USkillDataHandler::InitSkillVariable() {
	TArray<SArray*> SkillIndexMatrix;
	SkillIndexMatrix.Add(&AxeSkills);
	SkillIndexMatrix.Add(&GunSkills);

	for (auto Row : SkillIndexMatrix) {
		for (auto Element : *Row) {
			auto Origin = SkillVariableTable->FindRow<FSkillVariable>(Element->Skill_ID, "");
			Element->SkillParams = new SkillVariable;
			Element->SkillParams->Skill_ID = Origin->Skill_ID;
			Element->SkillParams->Variable01 = Origin->Variable01;
			Element->SkillParams->Variable02 = Origin->Variable02;
			Element->SkillParams->Variable03 = Origin->Variable03;
			Element->SkillParams->Variable04 = Origin->Variable04;
			Element->SkillParams->Variable05 = Origin->Variable05;
			Element->SkillParams->WeaponType = Origin->WeaponType;
			Element->SkillParams->SkillType = Origin->SkillType;
			Element->SkillParams->SkillName = Origin->SkillName;
			Element->SkillParams->Description = Origin->Description;
			Element->SkillParams->ThumbnailTexture = Origin->ThumbnailTexture;
		}
	}
}


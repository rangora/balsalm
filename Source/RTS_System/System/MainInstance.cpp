// Fill out your copyright notice in the Description page of Project Settings.


#include "MainInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Handler/SkillDataHandler.h"
#include "../DataTable/ABaseSkillTable.h"

UMainInstance::UMainInstance() {
	SkillDataHandler = NewObject<USkillDataHandler>();
}

void UMainInstance::Init() {
	Super::Init();

	//BaseSkillData_Info* skillslot = new BaseSkillData_Info;
	//skillslot = (AttackSkillData_Info*)skillslot;
	//SkillDataHandler->SetSkillData(SKILLTYPE::WEAPON_MELEE, 1, skillslot);
}

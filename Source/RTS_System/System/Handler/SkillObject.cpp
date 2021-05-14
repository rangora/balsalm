// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillObject.h"
#include "CoolDownHandler.h"
#include "SkillAnimHandler.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../ActorType.h"
#include "../../UI/QuickSlot.h"
#include "../../DataTable/ABaseSkillTable.h"



USkillObject::USkillObject() {
	CoolDownMgr = NewObject<UCoolDownHandler>();
}

void USkillObject::CheckSkillRange(AUnit* pUnit) {}

void USkillObject::ActiveSkill(AUnit* pUnit) {}

void USkillObject::SkillAction(AUnit* pUnit) {}

void USkillObject::Init() {
	CoolDownMgr->coolDownMax = 0.f;
	if(IsValid(SkillAnimMgr))
		CoolDownMgr->coolDownMax = SkillAnimMgr->SkillParams->Variable05;
	CoolDownMgr->currentCoolDown = 0.f;
	CoolDownMgr->CoolDownTrigger(false);
}

void USkillObject::Clone(USkillObject* SrcObject) {
	CoolDownMgr = NewObject<UCoolDownHandler>();
	SkillAnimMgr = SrcObject->SkillAnimMgr;
	CoolDownMgr->coolDownMax = SkillAnimMgr->SkillParams->Variable05;
	CoolDownMgr->currentCoolDown = SrcObject->CoolDownMgr->currentCoolDown;
	
	auto bCoolDown = SrcObject->CoolDownMgr->IsCoolDown();
	CoolDownMgr->CoolDownTrigger(bCoolDown);
}

FName USkillObject::GetID() {
	if(IsValid(SkillAnimMgr))
		return SkillAnimMgr->Skill_ID;
	return EMPTY_SKILL_ID;
}

UTexture2D* USkillObject::GetThumbnailTexture() {
	UTexture2D* output = SkillAnimMgr->SkillParams->ThumbnailTexture;
	return output;
}

float USkillObject::GetCoolDownMax() {
	if(IsValid(SkillAnimMgr))
		return SkillAnimMgr->SkillParams->Variable05;
	return 0.f;
}

float USkillObject::GetCurrentCoolDown() {
	return CoolDownMgr->currentCoolDown;
}

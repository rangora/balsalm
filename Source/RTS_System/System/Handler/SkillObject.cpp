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

void USkillObject::SkillActivator(AUnit* pUnit) {
	SkillAnimMgr->ActiveSkill(pUnit);
}

void USkillObject::CheckSkillRange(AUnit* pUnit) {
	SkillAnimMgr->CheckSkillRange(pUnit);
}

void USkillObject::SkillAction(AUnit* pUnit) {
	if (SkillCoolTimeAction.IsBound()) {
		SkillAnimMgr->PlaySkillAnimation(pUnit);
		SkillCoolTimeAction.Execute(true);
	}
}

void USkillObject::AreaSkillJudge(AUnit* pUnit) {
	SkillAnimMgr->AreaSkillJudge(pUnit);
}

SkillVariable* USkillObject::GetSkillParams() {
	return SkillAnimMgr->SkillParams;
}

void USkillObject::Init() {
	CoolDownMgr->coolDownMax = 0.f;
	if(IsValid(SkillAnimMgr))
		CoolDownMgr->coolDownMax = SkillAnimMgr->SkillParams->Variable05;
	CoolDownMgr->currentCoolDown = 0.f;
	CoolDownMgr->CoolDownTrigger(false);
}

void USkillObject::Clone(USkillObject* SrcObject) {
	CoolDownMgr = NewObject<UCoolDownHandler>();

	if (SrcObject->GetID() == EMPTY_SKILL_ID) {
		SkillAnimMgr = nullptr;
	}
	else {
		SkillAnimMgr = SrcObject->SkillAnimMgr;
		CoolDownMgr->coolDownMax = SkillAnimMgr->SkillParams->Variable05;
		SkillCoolTimeAction.BindUFunction(CoolDownMgr, FName("CoolDownTrigger"));
	}
}

void USkillObject::Clear() {
	SkillAnimMgr = nullptr;
}

void USkillObject::SetSkillAnimMgr(USkillAnimHandler* pSkillAnimMgr) {
	SkillAnimMgr = pSkillAnimMgr;
}

void USkillObject::SetSkillParams(SkillVariable* pSkillVariable) {
	SkillAnimMgr->SkillParams = pSkillVariable;
}

void USkillObject::ShowSkillArea(AUnit* pUnit, FVector CursorLocation) {
	SkillAnimMgr->ShowSkillArea(pUnit, CursorLocation);
}

void USkillObject::PlaySkillAnimation(AUnit* pUnit) {
	SkillAnimMgr->PlaySkillAnimation(pUnit);
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

bool USkillObject::bCoolDown() {
	return CoolDownMgr->IsCoolDown();
}


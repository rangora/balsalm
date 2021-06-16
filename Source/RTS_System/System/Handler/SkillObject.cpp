// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillObject.h"
#include "CoolDownHandler.h"
#include "SkillAnimHandler.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../ActorType.h"
#include "../../UI/QuickSlot.h"
#include "../../DataTable/ABaseSkillTable.h"
#include "../../Actors/Units/BaseMeleeUnit.h"



USkillObject::USkillObject() {
	CoolDownMgr = NewObject<UCoolDownHandler>();
}

void USkillObject::SkillActivator(AUnit* pUnit) {
	SkillActionMgr->ActiveSkill(pUnit);
}

void USkillObject::CheckSkillRange(AUnit* pUnit) {
	SkillActionMgr->CheckSkillRange(pUnit);
}

void USkillObject::SkillAction(AUnit* pUnit) {
	if (SkillCoolTimeAction.IsBound()) {
		SkillActionMgr->PlaySkillAnimation(pUnit);
		SkillCoolTimeAction.Execute(true);
		SkillActionMgr->bDrawRange = false;
	}
}

bool USkillObject::DoDrawSkillRange() {
	return SkillActionMgr->bDrawRange;
}

void USkillObject::AreaSkillJudge(AUnit* pUnit) {
	SkillActionMgr->AreaSkillJudge(pUnit);
}

SkillVariable* USkillObject::GetSkillParams() {
	return SkillActionMgr->SkillParams;
}

void USkillObject::Init() {
	CoolDownMgr->coolDownMax = 0.f;
	if(IsValid(SkillActionMgr))
		CoolDownMgr->coolDownMax = SkillActionMgr->SkillParams->Variable05;
	CoolDownMgr->currentCoolDown = 0.f;
	CoolDownMgr->CoolDownTrigger(false);
}

void USkillObject::Clone(USkillObject* SrcObject) {
	CoolDownMgr = NewObject<UCoolDownHandler>();

	if (SrcObject->GetID() == EMPTY_SKILL_ID) {
		SkillActionMgr = nullptr;
	}
	else {
		SkillActionMgr = SrcObject->SkillActionMgr;
		CoolDownMgr->coolDownMax = SkillActionMgr->SkillParams->Variable05;
		SkillCoolTimeAction.BindUFunction(CoolDownMgr, FName("CoolDownTrigger"));
	}
}

void USkillObject::Clear() {
	SkillActionMgr = nullptr;
}

void USkillObject::SetSkillAnimMgr(USkillAnimHandler* pSkillAnimMgr) {
	SkillActionMgr = pSkillAnimMgr;
}

void USkillObject::SetSkillParams(SkillVariable* pSkillVariable) {
	SkillActionMgr->SkillParams = pSkillVariable;
}

void USkillObject::ShowSkillArea(AUnit* pUnit, FVector CursorLocation) {
	SkillActionMgr->ShowSkillArea(pUnit, CursorLocation);
}

void USkillObject::PlaySkillAnimation(AUnit* pUnit) {
	SkillActionMgr->PlaySkillAnimation(pUnit);
}

FName USkillObject::GetID() {
	if(IsValid(SkillActionMgr))
		return SkillActionMgr->Skill_ID;
	return EMPTY_SKILL_ID;
}

UTexture2D* USkillObject::GetThumbnailTexture() {
	UTexture2D* output = SkillActionMgr->SkillParams->ThumbnailTexture;
	return output;
}



float USkillObject::GetCoolDownMax() {
	if(IsValid(SkillActionMgr))
		return SkillActionMgr->SkillParams->Variable05;
	return 0.f;
}

float USkillObject::GetCurrentCoolDown() {
	return CoolDownMgr->currentCoolDown;
}

bool USkillObject::bCoolDown() {
	return CoolDownMgr->IsCoolDown();
}


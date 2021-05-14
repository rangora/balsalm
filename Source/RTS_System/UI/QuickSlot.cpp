// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlot.h"
#include "SkillControlUI.h"
#include "WeaponSkillSlot.h"
#include "UMG/Public/Blueprint/WidgetTree.h"
#include "UMG/Public/Components/GridPanel.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "../System/Handler/SkillObject.h"
#include "../System/Handler/SkillAnimHandler.h"
#include "../ActorType.h"



void UQuickSlot::GetRefSkillObject(USkillObject* sObject) {
	if (!IsValid(sObject)) return;

	if (sObject->GetID() == EMPTY_SKILL_ID) {
		SetDefaultThumbnailImage();
		SkillObject = nullptr;
	}
	else {
		SkillObject = sObject;
		SkillObject->SkillAnimMgr->SkillCoolTimeAction.BindUFunction(this, FName("ActiveCooldown"));
		CurrentTexture = SkillObject->GetThumbnailTexture();
		SetThumbnailImage();
	}
}
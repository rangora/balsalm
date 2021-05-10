// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlot.h"
#include "SkillControlUI.h"
#include "WeaponSkillSlot.h"
#include "UMG/Public/Blueprint/WidgetTree.h"
#include "UMG/Public/Components/GridPanel.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "../System/Handler/SkillObject.h"
#include "../ActorType.h"



void UQuickSlot::GetRefSkillObject(USkillObject* sObject) {
	if (!IsValid(sObject)) return;

	if (sObject->Skill_ID == EMPTY_SKILL_ID) {
		SetDefaultThumbnailImage();
		SkillObject = nullptr;
		cooldownMax = 0.f;
		sObject->SkillCoolTimeAction.Unbind();
	}
	else {
		SkillObject = sObject;
		CurrentTexture = SkillObject->SkillParams->ThumbnailTexture;
		cooldownMax = SkillObject->SkillParams->Variable05;

		sObject->SkillCoolTimeAction.BindUFunction(this, FName("ActiveCooldown"));

		SetThumbnailImage();
	}
}
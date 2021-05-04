// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenUI.h"
#include "QuickSlot.h"
#include "../ActorType.h"
#include "../Actors/Units/BaseMeleeUnit.h"
#include "../Actors/Units/Component/ArmStatComponent.h"
#include "../System/Handler/SkillObject.h"
#include "../DataTable/ABaseSkillTable.h"
#include "UMG/Public/Components/GridPanel.h"
#include "UMG/Public/Blueprint/WidgetTree.h"

UScreenUI::UScreenUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SkillPanel = nullptr;
}

void UScreenUI::NativeConstruct() {
	Super::NativeConstruct();

	SkillPanel = this->WidgetTree->FindWidget<UGridPanel>("SkillPanel");
	if (SkillPanel) 
		SkillPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UScreenUI::NativeTick(const FGeometry& MyGeometry, float delta) {
	Super::NativeTick(MyGeometry, delta);
}

void UScreenUI::SetSkillPanelVisibility(bool trigger) {
	if (trigger) 
		SkillPanel->SetVisibility(ESlateVisibility::Visible);
	else 
		SkillPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UScreenUI::UpdateSkillSlots(const AUnit* pUnit) {
	auto AllyUnit = Cast<ABaseMeleeUnit>(pUnit);
	
	TArray<USkillObject*>& UnitSkillArray = AllyUnit->ArmStatComponent->SkillArray;
	auto SkillPanelSlots = SkillPanel->GetAllChildren();


	for (int idx = 0; idx < UNIT_SKILLSLOT_LENGTH; idx++) {
		auto PanelSlot = Cast<UQuickSlot>(SkillPanelSlots[idx]);
		auto UnitSkillObject = UnitSkillArray[idx];

		if (IsValid(PanelSlot) && UnitSkillObject) {
			PanelSlot->GetRefSkillObject(UnitSkillObject);
		}
	}
}

void UScreenUI::CleanAllSlots() {
	auto SkillPanelSlots = SkillPanel->GetAllChildren();
	
	for (int idx = 0; idx < UNIT_SKILLSLOT_LENGTH; idx++) {
		auto PanelSlot = Cast<UQuickSlot>(SkillPanelSlots[idx]);
		PanelSlot->SkillObject = nullptr;
		PanelSlot->SetThumbnailImage();
	}
}

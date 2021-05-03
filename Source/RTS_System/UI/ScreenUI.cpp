// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenUI.h"
#include "QuickSlot.h"
#include "UMG/Public/Components/GridPanel.h"
#include "UMG/Public/Blueprint/WidgetTree.h"

UScreenUI::UScreenUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	SkillPanel = nullptr;
}

void UScreenUI::NativeConstruct() {
	Super::NativeConstruct();

	SkillPanel = this->WidgetTree->FindWidget<UGridPanel>("SkillPanel");
	if (SkillPanel) {
		SkillPanel->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogTemp, Log, TEXT("Hide Panel!!"));
	}
}

void UScreenUI::NativeTick(const FGeometry& MyGeometry, float delta) {
	Super::NativeTick(MyGeometry, delta);
}

void UScreenUI::SetSkillPanelVisibility(bool trigger) {
	if (trigger) 
		SkillPanel->SetVisibility(ESlateVisibility::Visible);
	else {
		SkillPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UScreenUI::UpdateSkillPanel() {
	for (int idx = 0; idx < 4; idx++) {
		auto tSlot = SkillPanel->GetChildAt(idx);
		auto QuickSlot = Cast<UQuickSlot>(tSlot);

		if (IsValid(QuickSlot)) {
			QuickSlot->UpdateLinkSlot();
		}
	}

}

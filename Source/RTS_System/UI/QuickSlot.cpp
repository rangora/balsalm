// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlot.h"
#include "SkillControlUI.h"
#include "WeaponSkillSlot.h"
#include "UMG/Public/Blueprint/WidgetTree.h"
#include "UMG/Public/Components/GridPanel.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "../System/Handler/SkillObject.h"

void UQuickSlot::UpdateLinkSlot() {
	auto IController = Cast<AMainController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	auto SkillControlPanel = IController->SkillControlUIWidget;
	auto SkillPanel = SkillControlPanel->WidgetTree->FindWidget<UGridPanel>("SkillPanel");
	
	if (IsValid(SkillPanel)) {
		OriginSlot = SkillPanel->GetChildAt(RefIndex);

		// Origin is a member pointer.
		if (IsValid(OriginSlot)) {
			auto LinkedSlot = Cast<UWeaponSkillSlot>(OriginSlot);

			if (IsValid(LinkedSlot)) {

				if (LinkedSlot->bAssigned) {
					GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, TEXT("Skill Active!!"));
					LinkedSlot->SkillObject->ActiveSkill();
					CurrentTexture = LinkedSlot->CurrentTexture;
					SetThumbnailImage();
				}
				else {
					SetDefaultThumbnailImage();
					LinkedSlot->bAssigned = false;
					OriginSlot = nullptr;
				}
			}

		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillControlUI.h"
#include "../System/MainController.h"
#include "../ActorType.h"
#include "../System/MainInstance.h"
#include "../System/Handler/SkillDataHandler.h"
#include "../System/Handler/SkillObject.h"
#include "../Actors/Units/BaseMeleeUnit.h"
#include "../Actors/Units/Component/ArmStatComponent.h"
#include "CommonSlot.h"
#include "DragDropWidget.h"
#include "WeaponListSlot.h"
#include "WeaponSkillSlot.h"
#include "UMG/Public/Blueprint/WidgetTree.h"
#include "UMG/Public/Components/UniformGridPanel.h"
#include "UMG/Public/Components/GridPanel.h"
#include "Components/VerticalBox.h"




/* In-build functions. */

USkillControlUI::USkillControlUI(const FObjectInitializer& Initializer)
	: Super(Initializer), 
	SelectedWeapon(WEAPONTYPE::NONE),
	UsedWeapon(WEAPONTYPE::NONE) {

}

void USkillControlUI::NativeConstruct() {
	Super::NativeConstruct();

	bIsFocusable = true;
	SetKeyboardFocus();
	WeaponPanelInit();
}

/************************/


/* User-defined functions. */

bool USkillControlUI::OpenOrClose() {
	if (IsInViewport()) {
		RemoveFromViewport();
		return false;
	}
	else AddToViewport(1);
	return true;
}

void USkillControlUI::UpdateWeaponSkillList() {
	auto WeaponSkillPanel = WidgetTree->FindWidget<UUniformGridPanel>("WeaponSkillList");
	auto WeaponSkillSlots = WeaponSkillPanel->GetAllChildren();
	auto IInstance = Cast<UMainInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto SkillHandler = IInstance->SkillDataHandler;

	SkillHandler->UpdateWeaponSkillSlot(WeaponSkillSlots, SelectedWeapon);
}

void USkillControlUI::Clear() {
	auto SkillPanel = WidgetTree->FindWidget<UGridPanel>("SkillPanel");
	auto WeaponSkillPanel = WidgetTree->FindWidget<UUniformGridPanel>("WeaponSkillList");
	auto WeaponPanel = WidgetTree->FindWidget<UVerticalBox>("WeaponList");

	auto SkillArray = SkillPanel->GetAllChildren();
	auto WeaponSkillArray = WeaponSkillPanel->GetAllChildren();
	auto WeaponArray = WeaponPanel->GetAllChildren();


	auto _weaponSlot = Cast<UWeaponListSlot>(SkillArray[0]);
	_weaponSlot->SetDefaultThumbnailImage();
	_weaponSlot->bAssigned = false;

	for (int i = 1; i < SkillArray.Num(); i++) {
		auto _Slot = Cast<UWeaponSkillSlot>(SkillArray[i]);
		
		if (IsValid(Slot)) {
			_Slot->bAssigned = false;
			_Slot->SkillObject = nullptr;
			_Slot->SetDefaultThumbnailImage();
		}
	}

	for (int i = 0; i < WeaponSkillArray.Num(); i++) {
		auto _Slot = Cast<UWeaponSkillSlot>(WeaponSkillArray[i]);

		if (IsValid(_Slot)) {
			_Slot->bAssigned = false;
			_Slot->SkillObject = nullptr;
			_Slot->SetDefaultThumbnailImage();
		}
	}

	for (int i = 1; i <= WeaponArray.Num() - 1; i++) {
		auto _Slot = Cast<UWeaponListSlot>(WeaponArray[i]);

		if (IsValid(_Slot)) {
			_Slot->bBorderImageVisible(false);
		}
	}

	SelectedWeapon = WEAPONTYPE::NONE;
}

void USkillControlUI::UnitSkillConnector(const ABaseMeleeUnit* pUnit, const bool bOpenUI) {
	// Get selected skill list.
	// 1 ~ 4
	auto SkillPanel = WidgetTree->FindWidget<UGridPanel>("SkillPanel");
	
	if (bOpenUI) {
		// get unit skill data and show.
		GetUnitSkillData(pUnit, SkillPanel);
	}
	else {
		// update unit skill data..
		UpdateUnitSkillData(pUnit, SkillPanel);
	}
}

void USkillControlUI::ClearUserWeaponSkillSlot() {
	auto SkillPanel = WidgetTree->FindWidget<UGridPanel>("SkillPanel");
	auto Slots = SkillPanel->GetAllChildren();

	for (int idx = 1; idx <= UNIT_SKILLSLOT_LENGTH; idx++) {
		auto wSlot = Cast<UWeaponSkillSlot>(Slots[idx]);
		wSlot->SkillObject = nullptr;
		wSlot->SetDefaultThumbnailImage();
	}
}

/************************/


/* Private functions. */

void USkillControlUI::WeaponPanelInit() {
	auto Panel = WidgetTree->FindWidget<UVerticalBox>("WeaponList");
	auto WeaponArray = Panel->GetAllChildren();

	for (int i = 0; i < WeaponThumbnailPaths.Num(); i++) {
		// Add one value due to spacer(padding).
		auto WeaponSlot = Cast<UWeaponListSlot>(WeaponArray[i+1]);
		if (IsValid(WeaponSlot)) {
			WeaponSlot->CurrentTexture = LoadObject<UTexture2D>(NULL, *(WeaponThumbnailPaths[i]), NULL, LOAD_None, NULL);
			WeaponSlot->SetThumbnailImage();
			WeaponSlot->WeaponType = WeaponTypeList[i];
			WeaponSlot->bAssigned = true;
			WeaponSlot->bDragable = true;
			WeaponSlotArray.Add(WeaponSlot);
		}
	}
}

void USkillControlUI::GetUnitSkillData(const ABaseMeleeUnit* pUnit, const UGridPanel* SkillPanel) {
	TArray<USkillObject*>& UnitSkillObjects = pUnit->ArmStatComponent->SkillArray;
	auto SkillPanel_Slots = SkillPanel->GetAllChildren();
	
	// Weapon sync.
	auto UI_weaponSlot = Cast<UWeaponListSlot>(SkillPanel_Slots[0]);
	UI_weaponSlot->SetThisWeapon(pUnit->ArmStatComponent->UnitWeapon);

	// Weapon skill sync.
	for (int idx = 1; idx <= 4; idx++) {
		auto UI_SkillSlot = Cast<UWeaponSkillSlot>(SkillPanel_Slots[idx]);

		if (UnitSkillObjects[idx - 1]->Skill_ID != EMPTY_SKILL_ID) {
			UI_SkillSlot->SkillObject = UnitSkillObjects[idx - 1];
			UI_SkillSlot->CurrentTexture = UnitSkillObjects[idx - 1]->SkillParams->ThumbnailTexture;
			UI_SkillSlot->SetThumbnailImage();
		}
		else {
			UI_SkillSlot->SkillObject = nullptr;
			UI_SkillSlot->SetDefaultThumbnailImage();
		}
	}
}

void USkillControlUI::UpdateUnitSkillData(const ABaseMeleeUnit* pUnit, const UGridPanel* SkillPanel) {
	TArray<USkillObject*>& UnitSkillObjects = pUnit->ArmStatComponent->SkillArray;
	auto SkillPanel_Slots = SkillPanel->GetAllChildren();

	// Weapon update.
	auto UI_weaponSlot = Cast<UWeaponListSlot>(SkillPanel_Slots[0]);
	pUnit->ArmStatComponent->UnitWeapon = UI_weaponSlot->WeaponType;

	// Weapon skill update.
	for (int idx = 1; idx <= 4; idx++) {
		auto UI_SkillSlot = Cast<UWeaponSkillSlot>(SkillPanel_Slots[idx]);
		
		
		if (IsValid(UI_SkillSlot)) {
			if (UI_SkillSlot->SkillObject!=nullptr) {
				UnitSkillObjects[idx - 1]->Clone(UI_SkillSlot->SkillObject);
			}
			else {
				UnitSkillObjects[idx - 1]->Skill_ID = EMPTY_SKILL_ID;
			}
		}

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
			FString::Printf(TEXT("update:%s"), *UnitSkillObjects[idx-1]->Skill_ID.ToString()));

	}


}

/************************/


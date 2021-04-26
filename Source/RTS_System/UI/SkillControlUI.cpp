// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillControlUI.h"
#include "../System/MainController.h"
#include "../ActorType.h"
#include "../System/MainInstance.h"
#include "../System/Handler/SkillDataHandler.h"
#include "CommonSlot.h"
#include "DragDropWidget.h"
#include "WeaponListSlot.h"
#include "UMG/Public/Blueprint/WidgetTree.h"
#include "UMG/Public/Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"



bool USkillControlUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	auto Oper = Cast<UDragDropWidget>(InOperation);
	if (Oper == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Skill UI : NativeOnDrop  nullptr@@"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Skill UI : NativeOnDrop No NULL"));

	}

	return true;
}

void USkillControlUI::OpenOrClose() {
	if (IsInViewport()) RemoveFromViewport();
	else AddToViewport(1);
}

void USkillControlUI::UpdateWeaponSkillList() {
	auto WeaponSkillPanel = WidgetTree->FindWidget<UUniformGridPanel>("WeaponSkillList");
	auto WeaponSkillSlots = WeaponSkillPanel->GetAllChildren();
	auto IInstance = Cast<UMainInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto SkillHandler = IInstance->SkillDataHandler;

	switch (SelectedWeapon) {
	case WEAPONTYPE::SWORD: 
		break;
	case WEAPONTYPE::AXE:
		SkillHandler->SetAxeSkillData(WeaponSkillSlots);
		break;
	case WEAPONTYPE::BOW:
		break;
	case WEAPONTYPE::GUN:
		SkillHandler->SetGunSkillData(WeaponSkillSlots);
		break;
	default :
		break;
	}
	//
	//if (IsValid(WeaponSkillPanel)) {
	//	auto WeaponSkillSlots = WeaponSkillPanel->GetAllChildren();
	//	auto IInstance = Cast<UMainInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//	auto SkillHandler = IInstance->SkillDataHandler;

	//	if (IsValid(SkillHandler)) 
	//		SkillHandler->SetAxeSkillData(WeaponSkillSlots);
	//}
}

void USkillControlUI::WeaponPanelInit() {
	TArray<FString> ThumbnailPaths = {
	"/Game/Resources/Thumbnail/Skill/axe_thumbnail01.axe_thumbnail01",
	"/Game/Resources/Thumbnail/Skill/rifle_thumbnail01.rifle_thumbnail01"
	};
	TArray<WEAPONTYPE> TypeList = {
		WEAPONTYPE::AXE,
		WEAPONTYPE::GUN
	};

	auto Panel = WidgetTree->FindWidget<UVerticalBox>("WeaponList");
	auto WeaponArray = Panel->GetAllChildren();

	for (int i = 0; i < ThumbnailPaths.Num(); i++) {
		auto WeaponSlot = Cast<UWeaponListSlot>(WeaponArray[i+1]);
		if (IsValid(WeaponSlot)) {
			WeaponSlot->CurrentTexture = LoadObject<UTexture2D>(NULL, *(ThumbnailPaths[i]), NULL, LOAD_None, NULL);
			WeaponSlot->SetThumbnailImage();
			WeaponSlot->WeaponType = TypeList[i];
			WeaponSlot->bAssigned = true;
			WeaponSlotArray.Add(WeaponSlot);
		}
	}
}

void USkillControlUI::NativeConstruct() {
	Super::NativeConstruct();

	bIsFocusable = true;
	SetKeyboardFocus();
	WeaponPanelInit();
}


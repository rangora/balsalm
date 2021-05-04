// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponListSlot.h"
#include "DragDropWidget.h"
#include "SkillControlUI.h"
#include "../System/MainController.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"


/* In-build functions. */

void UWeaponListSlot::NativeConstruct() {
	Super::NativeConstruct();
	auto IController = Cast<AMainController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	WidgetLink = IController->SkillControlUIWidget;
}

FReply UWeaponListSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (!bAssigned) return reply.NativeReply;

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true) {
		auto SkillBox = Cast<UVerticalBox>(GetParent());
		if (IsValid(SkillBox)) {
			if (SelectedSlot == nullptr) {
				bBorderImageVisible(true);
				SelectedSlot = this;
			}
			else if (SelectedSlot != this) {
				SelectedSlot->bBorderImageVisible(false);
				bBorderImageVisible(true);
				SelectedSlot = this;
			}
			else return reply.NativeReply;
		}
	}
	WidgetLinkOperation();
		
	return reply.NativeReply;
}

/******************/


/* User defined functions. */

void UWeaponListSlot::bBorderImageVisible(bool visibility) {
	if (visibility)	BorderImage->SetVisibility(ESlateVisibility::Visible);
	else BorderImage->SetVisibility(ESlateVisibility::Hidden);
}

void UWeaponListSlot::SetThisWeapon(WEAPONTYPE pWeaponType) {
	WeaponType = pWeaponType;
	
	if (WeaponType == WEAPONTYPE::NONE) {
		SetDefaultThumbnailImage();
		bAssigned = false;
	}
	else {
		FString TexPath;

		switch (WeaponType) {
		case WEAPONTYPE::AXE: {
			TexPath = WeaponThumbnailPaths[WEAPON_INDEX::AXE];
			break;
		}
		case WEAPONTYPE::GUN: {
			TexPath = WeaponThumbnailPaths[WEAPON_INDEX::GUN];
			break;
		}
		}

		CurrentTexture = LoadObject<UTexture2D>(NULL, *(TexPath), NULL, LOAD_None, NULL);
		SetThumbnailImage();
		bAssigned = true;
	}
}

void UWeaponListSlot::WidgetLinkOperation() {
	if (WidgetLink.IsValid()) {
		auto SkillPanel = Cast<USkillControlUI>(WidgetLink.Get());
		if (IsValid(SelectedSlot)) {
			SkillPanel->SelectedWeapon = SelectedSlot->WeaponType;
			SkillPanel->UpdateWeaponSkillList();
		}
	}
}

void UWeaponListSlot::DropAction(const UUserWidget* From) {
	if (From->IsA(UWeaponListSlot::StaticClass())) {
		auto FromSlot = Cast<UWeaponListSlot>(From);
		auto SkillPanel = Cast<USkillControlUI>(WidgetLink.Get());

		// New weanon type..
		if (WeaponType != FromSlot->WeaponType) {
			SkillPanel->ClearUserWeaponSkillSlot();
		}

		// Get ref.
		CurrentTexture = FromSlot->CurrentTexture;
		WeaponType = FromSlot->WeaponType;
		SetThumbnailImage();
		
		SkillPanel->UsedWeapon = WeaponType;
	}
}

/****************/
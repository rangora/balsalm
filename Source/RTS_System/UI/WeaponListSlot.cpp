// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponListSlot.h"
#include "SkillControlUI.h"
#include "../System/MainController.h"
#include "Components/VerticalBox.h"

void UWeaponListSlot::bBorderImageVisible(bool visibility) {
	if (visibility)	BorderImage->SetVisibility(ESlateVisibility::Visible);
	else BorderImage->SetVisibility(ESlateVisibility::Hidden);
}

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
			else if(SelectedSlot!=this) {
				SelectedSlot->bBorderImageVisible(false);
				bBorderImageVisible(true);
				SelectedSlot = this;
			}
		}
	}
	WidgetLinkOperation();
		
	return reply.NativeReply;
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSkillSlot.h"
#include "DragDropWidget.h"
#include "Engine/Texture2D.h"

UWeaponSkillSlot::~UWeaponSkillSlot() {
	if (SkillData != nullptr)
		delete SkillData;
}

void UWeaponSkillSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) {
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (bAssigned) {
		auto Oper = Cast<UDragDropWidget>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropWidget::StaticClass()));
		auto VisualClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *VisualWidgetPath));

		if (IsValid(VisualClass)) {
			auto visual = Cast<UCommonSlot>(CreateWidget<UUserWidget>(GetWorld(), VisualClass));
			visual->CurrentTexture = this->SkillData->Thumbnail;
			visual->SetThumbnailImage();
			Oper->DefaultDragVisual = visual;
			OutOperation = Oper;
		}
	}

}

FReply UWeaponSkillSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (!bAssigned) return reply.NativeReply;

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true) {
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return reply.NativeReply;
}

FReply UWeaponSkillSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	FEventReply reply;

	return reply.NativeReply;
}

bool UWeaponSkillSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	auto Oper = Cast<UDragDropWidget>(InOperation);
	if (Oper!=nullptr) {
	}
	
	return false;
}

void UWeaponSkillSlot::SetThumbnailImage() {
	ThumbnailImage->SetBrushFromTexture(SkillData->Thumbnail);
}

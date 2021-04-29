// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonSlot.h"
#include "DragDropWidget.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"


void UCommonSlot::SetThumbnailImage() {
	if (!IsValid(CurrentTexture)) return;
	ThumbnailImage->SetBrushFromTexture(CurrentTexture);
}

void UCommonSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) {
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (bDragable) {
		auto Oper = Cast<UDragDropWidget>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropWidget::StaticClass()));
		auto VisualClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *VisualWidgetPath));

		if (IsValid(VisualClass)) {
			auto visual = Cast<UCommonSlot>(CreateWidget<UUserWidget>(GetWorld(), VisualClass));
			visual->CurrentTexture = CurrentTexture;
			visual->SetThumbnailImage();
			Oper->DefaultDragVisual = visual;
			Oper->From = this;
			OutOperation = Oper;
		}
	}
}

bool UCommonSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	auto Oper = Cast<UDragDropWidget>(InOperation);
	auto FromSlot = Cast<UCommonSlot>(Oper->From);
	
	if (!IsValid(FromSlot)) return false;
	
	if (FromSlot->bDragable && bOverWrite) 
		DropAction(Oper->From);

	return true;
}

FReply UCommonSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (!bAssigned) return reply.NativeReply;

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true) {
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return reply.NativeReply;
}

void UCommonSlot::WidgetLinkOperation() {}

void UCommonSlot::OverWrite(const UUserWidget* Origin) {}

void UCommonSlot::DropAction(const UUserWidget* From) {}

void UCommonSlot::SetDefaultThumbnailImage() {
	// TEST
	auto Tex = LoadObject<UTexture2D>(NULL, *DefaultTexturePath, NULL, LOAD_None, NULL);
	ThumbnailImage->SetBrushFromTexture(Tex);
}

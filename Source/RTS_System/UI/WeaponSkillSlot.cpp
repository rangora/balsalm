// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSkillSlot.h"
#include "DragDropWidget.h"
#include "SkillControlUI.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "../System/MainController.h"
#include "../System/Handler/SkillObject.h"

UWeaponSkillSlot::~UWeaponSkillSlot() {}

void UWeaponSkillSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) {
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

FReply UWeaponSkillSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (!bAssigned) return reply.NativeReply;

	return reply.NativeReply;
}

FReply UWeaponSkillSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	return reply.NativeReply;
}

void UWeaponSkillSlot::OverWrite(const UUserWidget* Origin) {
	auto Fromslot = Cast<UWeaponSkillSlot>(Origin);
	SkillObject = Fromslot->SkillObject;
	CurrentTexture = Fromslot->CurrentTexture;
	SetThumbnailImage();
}

void UWeaponSkillSlot::DropAction(const UUserWidget* From) {
	if (From->IsA(UWeaponSkillSlot::StaticClass())) {
		auto IController = Cast<AMainController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		auto SkillPanel = Cast<USkillControlUI>(IController->SkillControlUIWidget);
		auto FromSlot = Cast<UWeaponSkillSlot>(From);

		if (SkillPanel->UsedWeapon == FromSlot->WeaponType) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("UWeaponListSlot::SameWeaponType"));
			
			OverWrite(FromSlot);
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, 
				FString::Printf(TEXT("UWeaponListSlot::%f"), SkillObject->SkillParams->Variable01));

		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("UWeaponListSlot::DiffWeaponType"));
		}
	}
}

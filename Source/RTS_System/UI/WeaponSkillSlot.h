// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonSlot.h"
#include "../DataTable/ABaseSkillTable.h"
#include "WeaponSkillSlot.generated.h"

/**
 * 
 */

static FString VisualWidgetPath = "/Game/UI/UI_Slot.UI_Slot_C";

UCLASS()
class RTS_SYSTEM_API UWeaponSkillSlot : public UCommonSlot {
	GENERATED_BODY()

public:
	~UWeaponSkillSlot();
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


	virtual void SetThumbnailImage() override;

	BaseSkillData_Info* SkillData = nullptr;
};

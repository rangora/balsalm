// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonSlot.h"
#include "../DataTable/ABaseSkillTable.h"
#include "../ActorType.h"
#include "WeaponSkillSlot.generated.h"

/**
 * 
 */

class USkillObject;

UCLASS()
class RTS_SYSTEM_API UWeaponSkillSlot : public UCommonSlot {
	GENERATED_BODY()

public:
	~UWeaponSkillSlot();
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void OverWrite(const UUserWidget* Origin);

	virtual void DropAction(const UUserWidget* From) override;

	WEAPONTYPE WeaponType;
	int32 SkillIndex;
	// or
	UPROPERTY()
		USkillObject* SkillObject;
};



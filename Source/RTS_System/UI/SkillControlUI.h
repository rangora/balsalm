// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ActorType.h"
#include "Blueprint/UserWidget.h"
#include "SkillControlUI.generated.h"

/**
 * 
 */



UCLASS()
class RTS_SYSTEM_API USkillControlUI : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void OpenOrClose();
	void UpdateWeaponSkillList();

	UPROPERTY()
		TArray<UUserWidget*> WeaponSlotArray;

	WEAPONTYPE SelectedWeapon;

private:
	void WeaponPanelInit();
};

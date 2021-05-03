// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ActorType.h"
#include "Blueprint/UserWidget.h"
#include "SkillControlUI.generated.h"

/**
 * 
 */


class ABaseMeleeUnit;
class UGridPanel;

UCLASS()
class RTS_SYSTEM_API USkillControlUI : public UUserWidget {
	GENERATED_BODY()
public:
	USkillControlUI(const FObjectInitializer& Initializer);
	virtual void NativeConstruct() override;

	// Return true when display skillControlUI.
	bool OpenOrClose();
	void UpdateWeaponSkillList();
	void Clear();

	// Unit sync funcs.
	void UnitSkillConnector(const ABaseMeleeUnit* pUnit, const bool bOpenUI);

	UPROPERTY()
		TArray<UUserWidget*> WeaponSlotArray;

	WEAPONTYPE SelectedWeapon;
	WEAPONTYPE UsedWeapon;

private:
	void WeaponPanelInit();
	void GetUnitSkillData(const ABaseMeleeUnit* pUnit, const UGridPanel* SkillPanel);
	void UpdateUnitSkillData(const ABaseMeleeUnit* pUnit, const UGridPanel* SkillPanel);
};

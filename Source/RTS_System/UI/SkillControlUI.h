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
	USkillControlUI(const FObjectInitializer& Initializer);
	virtual void NativeConstruct() override;

	void OpenOrClose();
	void UpdateWeaponSkillList();

	UPROPERTY()
		TArray<UUserWidget*> WeaponSlotArray;

	WEAPONTYPE SelectedWeapon;
	WEAPONTYPE UsedWeapon;

private:
	void WeaponPanelInit();
};

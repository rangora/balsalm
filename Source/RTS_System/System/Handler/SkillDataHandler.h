// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "../../ActorType.h"
#include "UObject/NoExportTypes.h"
#include "SkillDataHandler.generated.h"

/**
 * 
 */


class UDataTable;
class UUserWidget;
class UWidget;


UCLASS()
class RTS_SYSTEM_API USkillDataHandler : public UObject {
	GENERATED_BODY()

	typedef TArray<USkillObject*> SArray;

public:
	USkillDataHandler();
	void UpdateWeaponSkillSlot(TArray<UWidget*>& Slots, const WEAPONTYPE WeaponType);
	

	UPROPERTY()
		TArray<USkillObject*> AxeSkills;
	UPROPERTY()
		TArray<USkillObject*> GunSkills;
	UPROPERTY()
		UDataTable* SkillVariableTable;

private:
	void InitSkillVariable();
	void SetWeaponSkilData(TArray<UWidget*>& Slots, const SArray* SkillArray, WEAPONTYPE WeaponType);
};

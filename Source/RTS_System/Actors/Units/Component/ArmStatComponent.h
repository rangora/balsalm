// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../../ActorType.h"
#include "ArmStatComponent.generated.h"

/**
 * 
 */

class USkillObject;

UCLASS()
class RTS_SYSTEM_API UArmStatComponent : public UObject {
	GENERATED_BODY()
public:
	UArmStatComponent();

	// Skill array initial size is eual to UNIT_SKILLSLOT_LENGTH(4)
	// and it can't be nullptr.
	UPROPERTY()
		TArray<USkillObject*> SkillArray;

	WEAPONTYPE UnitWeapon;
};

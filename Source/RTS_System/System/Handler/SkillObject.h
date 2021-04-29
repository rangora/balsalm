// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../ActorType.h"
#include "SkillObject.generated.h"

/**
 * 
 */

struct SkillVariable;

UCLASS()
class RTS_SYSTEM_API USkillObject : public UObject {
	GENERATED_BODY()
public:
	USkillObject();
	virtual void ActiveSkill();

	WEAPONTYPE WeaponType;
	SKILLTYPE SkillType;

	FName Skill_ID;
	FString SkillName;
	FString Description;
	float CoolTime;
	
	// Skill external data.
	SkillVariable* SkillParams;
};

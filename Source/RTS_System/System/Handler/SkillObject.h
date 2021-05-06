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
class UAnimInstance;
class AUnit;

UCLASS()
class RTS_SYSTEM_API USkillObject : public UObject {
	GENERATED_BODY()
public:
	USkillObject();
	virtual void ActiveSkill(AUnit* pUnit);

	FName Skill_ID;

	// This can't be nullptr.
	SkillVariable* SkillParams = nullptr;
};

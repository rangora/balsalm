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
	virtual void Clone(const USkillObject* From);

	FName Skill_ID;

	// This can't be nullptr.
	SkillVariable* SkillParams = nullptr;
};

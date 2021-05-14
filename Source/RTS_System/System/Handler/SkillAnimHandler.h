// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillAnimHandler.generated.h"

/**
 * 
 */

class AUnit;
struct SkillVariable;


DECLARE_DELEGATE(FSkillCoolTimeAction);



UCLASS()
class RTS_SYSTEM_API USkillAnimHandler : public UObject {
	GENERATED_BODY()

public:
	USkillAnimHandler();

	virtual void CheckSkillRange(AUnit* pUnit);
	virtual void ActiveSkill(AUnit* pUnit);
	virtual void SkillAction(AUnit* pUnit);
	
	FName Skill_ID;

	// Init in SkillDataHandler. (InitSkillVariable Func)
	SkillVariable* SkillParams = nullptr;

	FSkillCoolTimeAction SkillCoolTimeAction;
};

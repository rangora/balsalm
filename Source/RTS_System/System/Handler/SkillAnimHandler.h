// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UObject/NoExportTypes.h"
#include "../../Actors/Misc/AttackCaculator.h"
#include "../../System/MainInstance.h"
#include "SkillAnimHandler.generated.h"

/**
 * 
 */

class AUnit;
class USkillObject;
struct SkillVariable;




UCLASS()
class RTS_SYSTEM_API USkillAnimHandler : public UObject {
	GENERATED_BODY()

public:
	USkillAnimHandler();

	virtual void CheckSkillRange(AUnit* pUnit);
	/* It to be Called first by controller. */
	virtual void ActiveSkill(AUnit* pUnit);
	
	/* Called by HUD. */
	virtual void ShowSkillArea(AUnit* pUnit, FVector CursorLocation);
	
	virtual void PlaySkillAnimation(AUnit* pUnit);
	virtual void AreaSkillJudge(AUnit* pUnit);

	

	FName Skill_ID;
	// Init in SkillDataHandler. (InitSkillVariable Func)
	SkillVariable* SkillParams = nullptr;	
	
	bool bDrawRange = false;
};

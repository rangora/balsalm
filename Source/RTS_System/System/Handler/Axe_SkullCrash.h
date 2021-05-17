// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "SkillObject.h"
#include "SkillAnimHandler.h"
#include "Templates/SubclassOf.h"
#include "Axe_SkullCrash.generated.h"

/**
 * 
 */

class UAxeSkillAnimInstance;



UCLASS()
class RTS_SYSTEM_API UAxe_SkullCrash : public USkillAnimHandler {
	GENERATED_BODY()
public:
	UAxe_SkullCrash();
	
	/* A parameter, pUnit caller these functions. */
	void CheckSkillRange(AUnit* pUnit) override;
	void ActiveSkill(AUnit* pUnit) override;
	void PlaySkillAnimation(AUnit* pUnit) override;


	UPROPERTY()
		UAxeSkillAnimInstance* AxeSkillAnimInstance;
	UPROPERTY()
		TSubclassOf<UAnimInstance> AxeSkillAnimInstanceClass;
};

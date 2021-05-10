// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "SkillObject.h"
#include "Templates/SubclassOf.h"
#include "Axe_SkullCrash.generated.h"

/**
 * 
 */

class UAxeSkillAnimInstance;



UCLASS()
class RTS_SYSTEM_API UAxe_SkullCrash : public USkillObject {
	GENERATED_BODY()
public:
	UAxe_SkullCrash();
	
	void CheckSkillRange(ABaseMeleeUnit* pUnit);
	void virtual ActiveSkill(AUnit* pUnit) override;
	void SkillAction(ABaseMeleeUnit* pUnit) override;

	UPROPERTY()
		UAxeSkillAnimInstance* AxeSkillAnimInstance;
	UPROPERTY()
		TSubclassOf<UAnimInstance> AxeSkillAnimInstanceClass;
};

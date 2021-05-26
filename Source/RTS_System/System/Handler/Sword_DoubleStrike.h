// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillAnimHandler.h"
#include "Templates/SubclassOf.h"
#include "Animation/AnimInstance.h"
#include "Sword_DoubleStrike.generated.h"

/**
 * 
 */

class USwordSkillAnimInstance;


UCLASS()
class RTS_SYSTEM_API USword_DoubleStrike : public USkillAnimHandler {
	GENERATED_BODY()
public:
	USword_DoubleStrike();
	void ActiveSkill(AUnit* pUnit) override;
	void CheckSkillRange(AUnit* pUnit) override;
	void PlaySkillAnimation(AUnit* pUnit) override;

	UPROPERTY()
		USwordSkillAnimInstance* SwordSkillAnimInstance;
	UPROPERTY()
		TSubclassOf<UAnimInstance> SwordSkillAnimInstanceClass;
};

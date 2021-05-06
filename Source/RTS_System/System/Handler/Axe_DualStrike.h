// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "Axe_DualStrike.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API UAxe_DualStrike : public USkillObject {
	GENERATED_BODY()
public:
	UAxe_DualStrike();
	void virtual ActiveSkill(AUnit* pUnit) override;
};

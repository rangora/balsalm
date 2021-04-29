// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "Axe_SkullCrash.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API UAxe_SkullCrash : public USkillObject {
	GENERATED_BODY()
public:
	UAxe_SkullCrash();
	void virtual ActiveSkill() override;
};

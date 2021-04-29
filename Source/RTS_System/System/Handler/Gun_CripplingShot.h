// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "Gun_CripplingShot.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API UGun_CripplingShot : public USkillObject {
	GENERATED_BODY()
public:
	UGun_CripplingShot();
	void virtual ActiveSkill() override;
};

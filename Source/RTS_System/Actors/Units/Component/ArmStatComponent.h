// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ArmStatComponent.generated.h"

/**
 * 
 */

class USkillObject;

UCLASS()
class RTS_SYSTEM_API UArmStatComponent : public UObject {
	GENERATED_BODY()
public:

	void UpdateSkillData();

	UPROPERTY()
		TArray<USkillObject*> SkillArray;
};

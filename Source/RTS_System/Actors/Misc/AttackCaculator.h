// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttackCaculator.generated.h"

/**
 * 
 */

class AUnit;

UCLASS()
class RTS_SYSTEM_API UAttackCaculator : public UObject {
	GENERATED_BODY()

public:
	static float CaculateDamage(const AUnit* From, const AUnit* To);
};

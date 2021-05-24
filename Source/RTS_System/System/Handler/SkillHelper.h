// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillHelper.generated.h"



class ABaseMeleeUnit;



UCLASS()
class RTS_SYSTEM_API USkillHelper : public UObject {
	GENERATED_BODY()
public:
	void ShowSkillArea(ABaseMeleeUnit* pUnit);

};

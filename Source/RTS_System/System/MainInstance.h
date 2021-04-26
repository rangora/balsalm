// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainInstance.generated.h"

/**
 * 
 */

class UDataTable;
class USkillDataHandler;

UCLASS()
class RTS_SYSTEM_API UMainInstance : public UGameInstance {
	GENERATED_BODY()
public:
	UMainInstance();
	virtual void Init() override;

	UPROPERTY()
		USkillDataHandler* SkillDataHandler;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonSlot.h"
#include "QuickSlot.generated.h"

/**
 * 
 */

class USkillObject;

UCLASS()
class RTS_SYSTEM_API UQuickSlot : public UCommonSlot {
	GENERATED_BODY()
public:
	void GetRefSkillObject(USkillObject* sObject);

	UFUNCTION(BlueprintImplementableEvent)
		void ActiveCooldown();

	UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (BindWidget))
		UImage* Cooldown;

	UPROPERTY(BlueprintReadwrite)
		USkillObject* SkillObject = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../ActorType.h"
#include "UObject/NoExportTypes.h"
#include "SkillDataHandler.generated.h"

/**
 * 
 */


class UDataTable;
class UUserWidget;
struct BaseSkillData_Info;
struct AttackSkillData_Info;


UCLASS()
class RTS_SYSTEM_API USkillDataHandler : public UObject {
	GENERATED_BODY()
public:
	USkillDataHandler();
	
	void SetSkillData(SKILLTYPE skillType, int32 skill_id, BaseSkillData_Info* pSkillContainer);
	void SetAxeSkillData(TArray<UWidget*>& Slots);
	void SetGunSkillData(TArray<UWidget*>& Slots);


	UPROPERTY()
		UDataTable* AttackSkillTable;

private:
	void SetAttackSkillData(int32 skill_id, AttackSkillData_Info* pSkillContainer);

	TArray<int32> AxeSkillIndex;
	TArray<int32> GunSkillIndex;
};

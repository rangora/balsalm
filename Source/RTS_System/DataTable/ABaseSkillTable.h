// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "../ActorType.h"
#include "ABaseSkillTable.generated.h"


USTRUCT(BlueprintType)
struct FBaseSkillDataTable : public FTableRowBase {
	GENERATED_BODY()

	FBaseSkillDataTable();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
		int Skill_ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
		SKILLTYPE SkillType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
		FString SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
		float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
		UTexture2D* Thumbnail;
};

USTRUCT(BlueprintType)
struct FAttackSkillDataTable : public FBaseSkillDataTable {
	GENERATED_BODY()

	FAttackSkillDataTable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
		float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
		float DamageRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
		float AttackRange;
};

USTRUCT(BlueprintType)
struct FSpecifiedSkillTable : public FBaseSkillDataTable {
	GENERATED_BODY()

	FSpecifiedSkillTable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
		SKILLSHAPE SkillShape;
};



struct BaseSkillData_Info {
	int Skill_ID;
	SKILLTYPE SkillType;
	FString SkillName;
	FString Description;
	float CoolTime;
	UTexture2D* Thumbnail;
};

struct AttackSkillData_Info : public BaseSkillData_Info {
	float BaseDamage;
	float DamageRate;
	float AttackRange;
};

struct SpecifiedSkillTable : public AttackSkillData_Info {
	SKILLSHAPE SkillShape;
};
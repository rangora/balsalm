// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "../ActorType.h"
#include "ABaseSkillTable.generated.h"


USTRUCT(BlueprintType)
struct FSkillVariable : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) int32 Skill_ID = -1;
	UPROPERTY(EditAnywhere) float Variable01 = 0.f;
	UPROPERTY(EditAnywhere) float Variable02 = 0.f;
	UPROPERTY(EditAnywhere) float Variable03 = 0.f;
	UPROPERTY(EditAnywhere) float Variable04 = 0.f;
	UPROPERTY(EditAnywhere) float Variable05 = 0.f;
	UPROPERTY(EditAnywhere) WEAPONTYPE WeaponType;
	UPROPERTY(EditAnywhere) SKILLTYPE SkillType;
	UPROPERTY(EditAnywhere) FString SkillName;
	UPROPERTY(EditAnywhere) FString Description;
	UPROPERTY(EditAnywhere) UTexture2D* ThumbnailTexture; 
};

struct SkillVariable {
	int32 Skill_ID = -1;
	float Variable01;
	float Variable02;
	float Variable03;
	float Variable04;
	float Variable05;
	WEAPONTYPE WeaponType;
	SKILLTYPE SkillType;
	FString SkillName;
	FString Description;
	UTexture2D* ThumbnailTexture;
};
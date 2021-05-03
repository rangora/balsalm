// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseSkillAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API UBaseSkillAnimInstance : public UAnimInstance {
	GENERATED_BODY()
public:
	virtual void PlayAnimMontage();
};

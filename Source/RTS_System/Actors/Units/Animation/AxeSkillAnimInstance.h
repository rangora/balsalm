// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkillAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "AxeSkillAnimInstance.generated.h"

/**
 * 
 */


UCLASS()
class RTS_SYSTEM_API UAxeSkillAnimInstance : public UBaseSkillAnimInstance {
	GENERATED_BODY()
public:
	UAxeSkillAnimInstance();

	void PlaySkullCrack();
	void PlayCycloneAxe();

	UFUNCTION()
		void AnimNotify_SkillEnd();
	UFUNCTION()
		void AnimNotify_SkillCheck();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AxeSkill)
		UAnimMontage* SkullCrackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AxeSkill)
		UAnimMontage* CycloneAxeMontage;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* SkillCrack_Hit;
};

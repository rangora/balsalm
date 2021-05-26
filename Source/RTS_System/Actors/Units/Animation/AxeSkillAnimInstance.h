// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "AxeSkillAnimInstance.generated.h"

/**
 * 
 */


UCLASS()
class RTS_SYSTEM_API UAxeSkillAnimInstance : public UAnimInstance {
	GENERATED_BODY()
public:
	UAxeSkillAnimInstance();

	void PlaySkullCrack();
	void PlayCycloneAxe();

	UFUNCTION()
		void AnimNotify_SkillEnd();
	UFUNCTION()
		void AnimNotify_SkullCrackHit();
	UFUNCTION()
		void AnimNotify_CycloneAxeHit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AxeSkill)
		UAnimMontage* SkullCrackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AxeSkill)
		UAnimMontage* CycloneAxeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Paricle)
		UParticleSystemComponent* SkillCrack_Hit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Paricle)
		UParticleSystemComponent* CycloneAxe_Effect;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SwordSkillAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API USwordSkillAnimInstance : public UAnimInstance {
	GENERATED_BODY()
public:
	USwordSkillAnimInstance();

	void PlayDoubleStrike();

	UFUNCTION()
		void AnimNotify_DoubleStrike_FirstHit();
	UFUNCTION()
		void AnimNotify_DoubleStrike_SecondHit();
	UFUNCTION()
		void AnimNotify_SkillEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SwordSkill)
		UAnimMontage* DoubleStrikeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particle)
		UParticleSystemComponent* DoubleStrike_Hit_Effect;
};

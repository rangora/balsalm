// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitAnimInstance.h"
#include "MeleeAnimInstance.generated.h"

/**
 * 
 */



UCLASS()
class RTS_SYSTEM_API UMeleeAnimInstance : public UUnitAnimInstance {
	GENERATED_BODY()
	
public:
	UMeleeAnimInstance();

	void PlayBasicAttack();

	UFUNCTION()
		void AnimNotify_AttackHit();
	UFUNCTION()
		void AnimNotify_AttackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		UAnimMontage* BasicAttackMontage;
};

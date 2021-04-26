// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UnitAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API UUnitAnimInstance : public UAnimInstance {
	GENERATED_BODY()
public:
	UUnitAnimInstance();

	virtual void NativeUpdateAnimation(float delta) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MovementSpeed;
	
};

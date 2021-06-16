// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectAndAttack.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API UBTService_DetectAndAttack : public UBTService {
	GENERATED_BODY()

public:
	UBTService_DetectAndAttack();
	virtual void TickNode(UBehaviorTreeComponent& MinionComp, uint8* mem, float delta);
	
};

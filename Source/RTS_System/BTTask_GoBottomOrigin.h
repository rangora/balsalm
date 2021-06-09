// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GoBottomOrigin.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API UBTTask_GoBottomOrigin : public UBTTaskNode {
	GENERATED_BODY()
public:
	UBTTask_GoBottomOrigin();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& MinionComp, uint8* mem) override;

};

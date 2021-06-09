// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MinionController.generated.h"


class UBehaviorTree;
class UBlackboardData;


UCLASS()
class RTS_SYSTEM_API AMinionController : public AAIController {
	GENERATED_BODY()
public:
	AMinionController();

	virtual void OnPossess(APawn* MUnit) override;
	virtual void OnUnPossess() override;

	virtual void Tick(float delta) override;
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		UBehaviorTree* BT_Minion;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		UBlackboardData* BB_Minion;
};

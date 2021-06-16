// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MinionController.generated.h"


class UBehaviorTree;
class UBlackboardData;


static TArray<FName> BB_Minion_VectorValueName {
	"BottomOrigin",
	"EnemyHouse"};

static TArray<FVector> BB_EnemyMinion_VectorValue {   
	FVector(-220.f, 460.f, 0.f), 
	FVector(-370.f, -1800.f, 0.f) };

static TArray<FVector> BB_AllyMinion_VectorValue{
	FVector(-220.f, 460.f, 0.f),
	FVector(2300.f, 1800.f, 0.f) };


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

	static const bool bTracing;
};

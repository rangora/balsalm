// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GoBottomOrigin.h"
#include "Actors/Units/Minion.h"
#include "System/MinionController.h"
#include "Actors/Units/Component/AStarComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_GoBottomOrigin::UBTTask_GoBottomOrigin() {}

EBTNodeResult::Type UBTTask_GoBottomOrigin::ExecuteTask(UBehaviorTreeComponent& MinionComp, uint8* mem) {
	EBTNodeResult::Type Result = Super::ExecuteTask(MinionComp, mem);
	
	auto IMinion = Cast<AMinion>(MinionComp.GetAIOwner()->GetPawn());

	FVector Location = MinionComp.GetBlackboardComponent()->GetValueAsVector(TEXT("BottomOrigin"));

	IMinion->Astar->MoveToLocation(Location);
	Result = EBTNodeResult::Succeeded;

	return Result;
}

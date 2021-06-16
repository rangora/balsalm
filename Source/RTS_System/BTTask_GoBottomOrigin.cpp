// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GoBottomOrigin.h"
#include "Actors/Units/Minion.h"
#include "Actors/Units/Component/StatComponent.h"
#include "ActorType.h"
#include "System/MinionController.h"
#include "System/MainGameMode.h"
#include "Actors/Units/Component/AStarComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_GoBottomOrigin::UBTTask_GoBottomOrigin() {
	locationIdx = 0;
}

EBTNodeResult::Type UBTTask_GoBottomOrigin::ExecuteTask(UBehaviorTreeComponent& MinionComp, uint8* mem) {
	EBTNodeResult::Type Result = Super::ExecuteTask(MinionComp, mem);
	
	auto IMinion = Cast<AMinion>(MinionComp.GetAIOwner()->GetPawn());
	auto IWorld = IMinion->GetWorld();
	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	bool bTracing = MinionComp.GetBlackboardComponent()->GetValueAsBool(FName("bTracing"));

	if (bTracing || (IMinion->UnitStat->DeadOrAlive == DOA::DEAD)) {
		Result = EBTNodeResult::Failed;
	}
	else if (locationIdx < BB_EnemyMinion_VectorValue.Num()) {
		FVector DesiredLocation = BB_EnemyMinion_VectorValue[locationIdx];

		if (IMode->player_Team_Number == IMinion->unit_Team_Number) {
			DesiredLocation = BB_AllyMinion_VectorValue[locationIdx];
		}


		IMinion->Astar->MoveToLocation(DesiredLocation);

		TArray<FOverlapResult> Results;
		FCollisionObjectQueryParams QueryParam;

		FCollisionQueryParams CollisionQueryParam(NAME_None, false, IMinion);
		bool bHit = IWorld->OverlapMultiByObjectType(
			Results,
			DesiredLocation,
			FQuat::Identity,
			FCollisionObjectQueryParams::AllObjects,
			FCollisionShape::MakeSphere(50.f)
		);

		if (bHit) {
			for (auto Element : Results) {
				auto TUnit = Cast<AUnit>(Element.GetActor());

				if (!IsValid(TUnit)) continue;
					
				if (TUnit == IMinion) {
					locationIdx++;
					break;
				}
			}
		}
		Result = EBTNodeResult::Succeeded;
	}
	else {
		Result = EBTNodeResult::Failed;
	}

	return Result;
}

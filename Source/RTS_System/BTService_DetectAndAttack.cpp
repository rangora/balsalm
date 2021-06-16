// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectAndAttack.h"
#include "System/MinionController.h"
#include "Actors/Units/Component/StatComponent.h"
#include "ActorType.h"
#include "Actors/Units/Minion.h"
#include "BehaviorTree/BlackboardComponent.h"




UBTService_DetectAndAttack::UBTService_DetectAndAttack() {
	NodeName = TEXT("DetectAndAttack");
	Interval = 0.2f;
}

void UBTService_DetectAndAttack::TickNode(UBehaviorTreeComponent& MinionComp, uint8* mem, float delta) {
	Super::TickNode(MinionComp, mem, delta);

	auto IMinion = Cast<AMinion>(MinionComp.GetAIOwner()->GetPawn());
	auto IWorld = IMinion->GetWorld();
	
	if (IMinion->UnitStat->DeadOrAlive == DOA::DEAD) {
		return;
	}

	TArray<FOverlapResult> Results;
	FCollisionObjectQueryParams QueryParam;

	FCollisionQueryParams CollisionQueryParam(NAME_None, false, IMinion);
	CollisionQueryParam.AddIgnoredActor(IMinion);
	
	bool bHit = IWorld->OverlapMultiByObjectType(
		Results,
		IMinion->GetActorLocation(),
		FQuat::Identity,
		FCollisionObjectQueryParams::AllObjects,
		FCollisionShape::MakeSphere(500.f),
		CollisionQueryParam
	);
	
	if (bHit) {
		for (auto Element : Results) {
			auto TActor = Element.GetActor();
			auto TUnit = Cast<AUnit>(Element.GetActor());

			if (!IsValid(TUnit)) continue;

			if (TUnit->unit_Team_Number != IMinion->unit_Team_Number) {
				MinionComp.GetBlackboardComponent()->SetValueAsBool(FName("bTracing"), true);
				IMinion->StopMovement();
				IMinion->TargetUnits[0] = TUnit;
				IMinion->BasicAttack();

				return;
			}
		}
		MinionComp.GetBlackboardComponent()->SetValueAsBool(FName("bTracing"), false);
	}
	else {
		MinionComp.GetBlackboardComponent()->SetValueAsBool(FName("bTracing"), false);
	}
}

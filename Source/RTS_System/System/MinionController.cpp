// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "../Actors/Units/Unit.h"


AMinionController::AMinionController() {
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(
		TEXT("/Game/AI/BB_Minion.BB_Minion"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(
		TEXT("/Game/AI/BT_Minion.BT_Minion"));
	
	if (BBObject.Succeeded())
		BB_Minion = BBObject.Object;
	if (BTObject.Succeeded())
		BT_Minion = BTObject.Object;
}

void AMinionController::OnPossess(APawn* MUnit) {
	Super::OnPossess(MUnit);
	if (UseBlackboard(BB_Minion, Blackboard)) {
		Blackboard->SetValueAsVector(TEXT("BottomOrigin"), FVector(-220.f,460.f,0.f));

		RunBehaviorTree(BT_Minion);
	}
}

void AMinionController::OnUnPossess() {
}

void AMinionController::Tick(float delta) {
	Super::Tick(delta);
}

void AMinionController::BeginPlay() {
	Super::BeginPlay();
}


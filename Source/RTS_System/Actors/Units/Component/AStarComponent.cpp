// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarComponent.h"
#include "TimerManager.h"
#include "../../../System/UnitController.h"
#include "../../Misc/PathSphere.h"
#include "../Unit.h"
#include "AstarModule.h"

UAStarComponent::UAStarComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	bNeedToGo = false;
	bNextStep = true;
	bMoving = false;
}

UAStarComponent::~UAStarComponent() {
	delete Core;
}

void UAStarComponent::BeginPlay() {
	Super::BeginPlay();
	Core = new AstarPathFinding(GetOwner());

}

void UAStarComponent::InitializeComponent() {
	Super::InitializeComponent();
	Dele_PathFind.BindUFunction(this, FName("FollowThePath"));

	GetOwner()->GetWorldTimerManager().SetTimer(PathFindTimer,
		this, &UAStarComponent::PathFindTimerFunc, 0.2f, true);
}


void UAStarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bNeedToGo) {
		TArray<AActor*> PathArray;

		auto AllyUnit = Cast<AUnit>(GetOwner());
		AllyUnit->PathFollower->GetOverlappingActors(PathArray, APathSphere::StaticClass());

		// Arrive at a target path sphere.
		if (PathArray.Num()) {
			if (IsValid(PathArray[0])) {
				PathArray[0]->Destroy();
			}

			// So, get the next path sphere.
			bNextStep = true;

			// Check reach the final destination.
			if (LocationArray.Num() == 0) {
				bNextStep = bMoving = bNeedToGo = false;
			}
		}

		// Move to the next path sphere.
		if (bNextStep) {
			if (Dele_PathFind.IsBound()) {
				if (LocationArray.Num())
					Dele_PathFind.Execute();
				else
					ClearRoute();
			}
			bNextStep = false;
		}
	}
}

void UAStarComponent::MoveToLocation(const FVector& LocationValue) {
	auto InWorld = GetWorld();
	FVector UnitLocation = GetOwner()->GetActorLocation();
	

	if (bTimeToFinding) {
		ClearRoute();
		Core->Find(UnitLocation, LocationValue, LocationArray, InWorld);
		bTimeToFinding = false;

		int32 num = LocationArray.Num();
		if (num > 1) LocationArray.Pop();
		if (num) bMoving = bNextStep = bNeedToGo = true;
	}
}

void UAStarComponent::ClearRoute() {
	LocationArray.Empty();
	PathSpheres.Reset();
}

void UAStarComponent::PathFindTimerFunc() {
	bTimeToFinding = true;
}

void UAStarComponent::FollowThePath() {
	auto IUnit = Cast<AUnit>(GetOwner());
	auto AIController = Cast<AAIController>(IUnit->GetController());

	auto NextLocation = LocationArray.Pop();
	auto NewSphere = GetWorld()->SpawnActor<APathSphere>(APathSphere::StaticClass(), NextLocation, FRotator::ZeroRotator);

	PathSpheres.Add(NewSphere);
	AIController->MoveToLocation(NextLocation, -1.f, false, false);
}

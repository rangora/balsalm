// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarComponent.h"
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
}


// Called every frame
void UAStarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bNeedToGo) {
		TArray<AActor*> PathArray;
		GetOwner()->GetOverlappingActors(PathArray, APathSphere::StaticClass());

		if (PathArray.Num()) {
			if (IsValid(PathArray[0])) {
				PathArray[0]->Destroy();
			}
			bNextStep = true;
			// Got it.
			if (++PathSize > PathLocation.Num()) {
				PathSize = 1;
				bNextStep = bMoving = bNeedToGo = false;
			}
		}

		if (bNextStep) {
			if (Dele_PathFind.IsBound())
				Dele_PathFind.Execute(PathLocation.Num() - PathSize);
			//FollowThePath(PathLocation.Num() - PathSize);
			bNextStep = false;
		}
	}
}

void UAStarComponent::MoveToLocation(const FVector& LocationValue) {
	auto InWorld = GetWorld();
	PathLocation.Empty();

	for (auto Element : PathSpheres) {
		if (IsValid(Element) && !Element->IsPendingKill())
			Element->Destroy();
	}
	PathSpheres.Reset();

	Core->Find(GetOwner()->GetActorLocation(), LocationValue, PathLocation, InWorld);
	if (PathLocation.Num()) {
		PathSize = 1;
		bMoving = bNextStep = bNeedToGo = true;
	}
}

void UAStarComponent::FollowThePath(int32 idx) {
	auto IUnit = Cast<AUnit>(GetOwner());
	auto AIController = Cast<AAIController>(IUnit->GetController());
	auto NewSphere = GetWorld()->SpawnActor<APathSphere>(APathSphere::StaticClass(), PathLocation[idx], FRotator::ZeroRotator);
	PathSpheres.Add(NewSphere);
	AIController->MoveToLocation(PathLocation[idx], -1.f, false, false);
}

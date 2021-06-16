// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AStarComponent.generated.h"

class AstarPathFinding;
class APathSphere;

DECLARE_DELEGATE(FDelePathFind);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_SYSTEM_API UAStarComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UAStarComponent();
	~UAStarComponent();
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveToLocation(const FVector& LocationValue);
	void ClearRoute();

	UFUNCTION()
		void PathFindTimerFunc();

	UFUNCTION()
		void FollowThePath();

	bool bMoving;

private:
	UPROPERTY()
		TArray<APathSphere*> PathSpheres;
	UPROPERTY()
		TArray<FVector> LocationArray;
	
	bool bNeedToGo;
	bool bNextStep;
	bool bTimeToFinding = true;

	AstarPathFinding* Core;
	FDelePathFind Dele_PathFind;
	FTimerHandle PathFindTimer;

};

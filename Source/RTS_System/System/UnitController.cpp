// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitController.h"
#include "MainGameMode.h"
#include "../Actors/Units/Unit.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AUnitController::AUnitController() {}

void AUnitController::Tick(float delta) {
	Super::Tick(delta);
	//auto IPlayer = Cast<AUnit>(GetCharacter());
	//auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));


	//if (IsValid(IPlayer) && IsValid(IMode)) {
	//	auto IFow = IMode->PlayerFow;

	//	FVector2D Location2D{ IPlayer->GetActorLocation().X, IPlayer->GetActorLocation().Y };
	//	IFow->SetActorLocation(FVector(0.f, 0.f, IPlayer->GetActorLocation().Z));
	//	IFow->RevealSmoothCircle(Location2D, 600);
	//}
}

void AUnitController::BeginPlay() {
	Super::BeginPlay();
}

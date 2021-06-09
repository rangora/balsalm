// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "FogRegister.h"
#include "FogManager.h"
#include "../Actors/Units/Unit.h"
#include "Runtime/Engine/Public/EngineUtils.h"

AMainGameMode::AMainGameMode() :
	player_Team_Number(1) {
	PrimaryActorTick.bCanEverTick = false;
}

void AMainGameMode::Tick(float delta) {
	Super::Tick(delta);
}

void AMainGameMode::BeginPlay() {
	Super::BeginPlay();
}
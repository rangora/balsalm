// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
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

	//PlayerFow = GetWorld()->SpawnActor<AFow>(AFow::StaticClass(),
	//	FVector(0.f, 0.f, 600.f), FRotator::ZeroRotator);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSphere.h"

// Sets default values
APathSphere::APathSphere() {
	PrimaryActorTick.bCanEverTick = false;
	DestSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestSphere"));
	DestSphere->SetSphereRadius(30.f);
	DestSphere->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = DestSphere;
}



APathSphere::~APathSphere() {}

void APathSphere::BeginPlay() {
	Super::BeginPlay();
	SetActorHiddenInGame(true);
}

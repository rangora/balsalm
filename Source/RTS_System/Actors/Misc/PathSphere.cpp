// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSphere.h"
#include "Engine.h"

// Sets default values(24)
APathSphere::APathSphere() {
	PrimaryActorTick.bCanEverTick = false;
	DestSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestSphere"));
	StaticSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticSphere"));
	StaticSphere->SetVisibility(false);

	DestSphere->SetSphereRadius(30.f);

	ConstructorHelpers::FObjectFinder<UStaticMesh> ST_Circle(
		TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	if (ST_Circle.Succeeded())
		StaticSphere->SetStaticMesh(ST_Circle.Object);

	DestSphere->SetCollisionProfileName(TEXT("Trigger"));
	StaticSphere->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = StaticSphere;
}



APathSphere::~APathSphere() {}

void APathSphere::BeginPlay() {
	Super::BeginPlay();
}

void APathSphere::BeginDestroy() {
	Super::BeginDestroy();
}

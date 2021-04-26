// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource.h"

// Sets default values
AResource::AResource() 
	: current_amount(500) {
	PrimaryActorTick.bCanEverTick = false;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = Body;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionProfileName(TEXT("BlockAll"));
	TriggerBox->SetupAttachment(RootComponent);
}

void AResource::Exhausted_Implementation() {
}

void AResource::GetResource(int amount) {
	current_amount -= amount;

	if (current_amount <= 0)
		Exhausted();
}

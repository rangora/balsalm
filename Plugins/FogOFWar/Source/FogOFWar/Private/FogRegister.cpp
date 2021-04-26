// Fill out your copyright notice in the Description page of Project Settings.


#include "FogRegister.h"
#include "FogOfWarWorker.h"
#include "Engine/Public/EngineUtils.h"
#include "FogManager.h"


UFogRegister::UFogRegister() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UFogRegister::BeginPlay() {
	Super::BeginPlay();

	//FString ObjectName = GetOwner()->GetName();
	//UE_LOG(LogTemp, Log, TEXT("I am alive %s"), *ObjectName);

	////registering the actor to the FOW Manager

	//auto InWorld = GetWorld();
	//for (const auto& entity : TActorRange<AActor>(InWorld)) {
	//	auto IActor = Cast<AFogManager>(entity);
	//	if (IsValid(IActor)) {
	//		Manager = IActor;
	//		break;
	//	}
	//}


	//if (Manager != nullptr) {
	//	UE_LOG(LogTemp, Log, TEXT("Found Manager"));

	//	Manager->RegisterFowActor(GetOwner());
	//}
	//else {
	//	UE_LOG(LogTemp, Log, TEXT("Please attach a FOW Manager"));
	//}
}

void UFogRegister::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFogRegister::RegisterToManager() {
	auto InWorld = GetWorld();
	
	for (const auto& entity : TActorRange<AActor>(InWorld)) {
		auto IActor = Cast<AFogManager>(entity);
		if (IsValid(IActor)) {
			Manager = IActor;
			break;
		}
	}

	if (IsValid(Manager)) {
		Manager->RegisterFowActor(GetOwner());
	}
}


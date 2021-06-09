// Fill out your copyright notice in the Description page of Project Settings.


#include "FogRegister.h"
#include "FogOfWarWorker.h"
#include "Engine/Public/EngineUtils.h"
#include "FogManager.h"


UFogRegister::UFogRegister() {
	PrimaryComponentTick.bCanEverTick = true;
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


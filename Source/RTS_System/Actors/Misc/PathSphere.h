// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PathSphere.generated.h"

UCLASS()
class RTS_SYSTEM_API APathSphere : public AActor {
	GENERATED_BODY()
	
public:	
	APathSphere();
	~APathSphere();
	virtual void BeginPlay() override;

	USphereComponent* DestSphere;
};

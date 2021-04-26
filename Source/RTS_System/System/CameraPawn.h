// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "MainController.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

UCLASS()
class RTS_SYSTEM_API ACameraPawn : public APawn {
	GENERATED_BODY()

public:
	ACameraPawn();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		FVector2D CameraLocation;
};

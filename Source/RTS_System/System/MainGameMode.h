// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FogManager.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"



class AUnit;


UCLASS()
class RTS_SYSTEM_API AMainGameMode : public AGameModeBase {
	GENERATED_BODY()
	
public:
	AMainGameMode();
	virtual void Tick(float delta) override;
	virtual void BeginPlay() override;

	int player_Team_Number;
};

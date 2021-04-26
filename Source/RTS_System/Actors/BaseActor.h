// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.generated.h"

UCLASS()
class RTS_SYSTEM_API ABaseActor : public AActor {
	GENERATED_BODY()
	
public:	
	ABaseActor();
	
	UPROPERTY(Editanywhere, BlueprintReadWrite)
		bool bPassable;
};

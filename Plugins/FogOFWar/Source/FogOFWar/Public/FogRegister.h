// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FogRegister.generated.h"


class AFogManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOGOFWAR_API UFogRegister : public UActorComponent {
	GENERATED_BODY()

public:	
	UFogRegister();
	void RegisterToManager();

	/*Select the FOW Manager*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
		AFogManager* Manager = nullptr;
};

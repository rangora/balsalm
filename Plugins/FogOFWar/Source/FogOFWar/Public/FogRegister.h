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

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RegisterToManager();

	/*Select the FOW Manager*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
		AFogManager* Manager = nullptr;
	/*Is the actor able to influence unfogged texels*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
		bool WriteUnFog = true;
	/*Is the actor able to influence fog of war texels*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
		bool WriteFow = true;
	/*Is the actor able to influence terra incognita texels*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
		bool WriteTerraIncog = true;
	/*Check if the actor is in terra incognita*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
		bool bCheckActorTerraIncog = false;
	/**/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FogOfWar)
		bool isActorInTerraIncog = false;
};

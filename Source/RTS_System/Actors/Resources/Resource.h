// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Resource.generated.h"

UCLASS()
class RTS_SYSTEM_API AResource : public AActor {
	GENERATED_BODY()
	
public:	
	AResource();

	UFUNCTION(BlueprintNativeEvent)
		void Exhausted();

	virtual void Exhausted_Implementation();

	UFUNCTION(BlueprintCallable)
		void GetResource(int amount);

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		int current_amount;
};

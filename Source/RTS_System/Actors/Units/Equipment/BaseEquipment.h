// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../ActorType.h"
#include "GameFramework/Actor.h"
#include "BaseEquipment.generated.h"

UCLASS()
class RTS_SYSTEM_API ABaseEquipment : public AActor {
	GENERATED_BODY()
	
public:	
	ABaseEquipment();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		USkeletalMeshComponent* EquipmentSkeletal;
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		UStaticMeshComponent* EquipmentMesh;

	UPROPERTY(EditAnywhere, Category = ItemStat)
		ITEMTYPE ItemType;
};


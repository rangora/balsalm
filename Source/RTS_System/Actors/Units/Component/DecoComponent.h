// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "DecoComponent.generated.h"


class UHeadUpHPbar;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_SYSTEM_API UDecoComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UDecoComponent();
	virtual void BeginPlay() override;

	void UpdateAttackRadius();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* SelectionMeshRef;
	
	USphereComponent* AttackRadiusRef;

	UPROPERTY(VisibleAnywhere, Category = UI)
		UWidgetComponent* HeadUpHPbarRef;

	UMaterialInstanceDynamic* SelectionDynamic;

	UPROPERTY(Editanywhere, BlueprintReadWrite)
		bool bSelected = false;
};

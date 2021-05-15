// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../ActorType.h"
#include "StatComponent.generated.h"


DECLARE_DELEGATE(FDeleHPBarUpdate);
DECLARE_DELEGATE(FAfterDeadProcess);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_SYSTEM_API UStatComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UStatComponent();
	virtual void BeginPlay() override;
	void TakeDamage(float totalDamage);

	DOA DeadOrAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		int level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float maxHP;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
		float currentHP;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
		float currentDamage;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
		float attackRange;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
		float attackSpeed;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
		float Armor;

	FCriticalSection _mutex;

	FDeleHPBarUpdate Dele_HPWidgetUpdate;
	FAfterDeadProcess AfterDeadProcess;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit.h"
#include "Equipment/BaseWeapon.h"
#include "BaseMeleeUnit.generated.h"

/**
 * 
 */


class UMeleeAnimInstance;
class ABaseEquipment;
class UArmStatComponent;


UCLASS()
class RTS_SYSTEM_API ABaseMeleeUnit : public AUnit {
	GENERATED_BODY()

public:
	ABaseMeleeUnit();
	virtual void Tick(float delta) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Interaction_Implementation(const FVector& RB_Vector, AActor* Target);

	//void AttackCheck();
	//void TakeDamage(float amount);
	void BasicAttack();
	void BasicAttackEnd();

	void EquipmentMount(ABaseEquipment* Item);

	UPROPERTY()
		UMeleeAnimInstance* AnimInstance;
	UPROPERTY()
		UArmStatComponent* ArmStatComponent;


private:
	FCriticalSection _mutex;
	bool bReadyBasicAttack;
};

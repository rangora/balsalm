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

	void BasicAttack();
	void BasicAttackEnd();

	UFUNCTION()
		void SetBasicAnimInstance();

	void EquipmentMount(ABaseEquipment* Item);

	UPROPERTY()
		TSubclassOf<UAnimInstance> BasicUnitAnimInstanceClass;
	UPROPERTY() 
		UMeleeAnimInstance* AnimInstance;
	UPROPERTY()
		UArmStatComponent* ArmStatComponent;

	UPROPERTY(EditAnywhere)
		UDecalComponent* DecalSkillRange;

	bool bGoBasicAnimInstance = false;
	bool bMovable = true;

private:
	FCriticalSection _mutex;
	bool bReadyBasicAttack;
};

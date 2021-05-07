// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Unit.h"
#include "Equipment/BaseWeapon.h"
#include "BaseMeleeUnit.generated.h"

/**
 * 
 */


class UMeleeAnimInstance;
class ABaseEquipment;
class UArmStatComponent;
class USkillObject;


enum class UNIT_BEHAVIOR {
	NOTHING,
	MOVABLE,
	BASICATTACK_ORDER,
	ATTACKING,
	SKILL_TARGETING,
};


UCLASS()
class RTS_SYSTEM_API ABaseMeleeUnit : public AUnit {
	GENERATED_BODY()

public:
	ABaseMeleeUnit();
	virtual void Tick(float delta) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	
	// Mouse right button action.
	virtual void Interaction_Implementation(const FVector& RB_Vector, AActor* Target);

	void BasicAttack();
	void BasicAttackEnd();

	// Skill system.
	void AppointTheSkillTarget(float skillRange, USkillObject* ActivatedSkill);
	void SkillActivator();

	// Behavior controller.
	void SetDefaultBehavior();
	bool CheckBehavior(UNIT_BEHAVIOR var);
	void TurnOnBehavior(UNIT_BEHAVIOR var);
	void TurnOffBehavior(UNIT_BEHAVIOR var);

	UFUNCTION()
		void SetBasicAnimInstance();

	void EquipmentMount(ABaseEquipment* Item);

	UPROPERTY()
		TSubclassOf<UAnimInstance> BasicUnitAnimInstanceClass;
	UPROPERTY() 
		UMeleeAnimInstance* AnimInstance;
	UPROPERTY()
		UArmStatComponent* ArmStatComponent;

	UPROPERTY()
		USphereComponent* SkillRadius;
	UPROPERTY(EditAnywhere)
		UDecalComponent* DecalSkillRange;

	UPROPERTY()
		USkillObject* SkillRef = nullptr;

	float skillRadius;
	bool bGoBasicAnimInstance = false;
	bool bMovable = true;
	bool bForSkill = false;

private:
	FCriticalSection _mutex;
	UNIT_BEHAVIOR Behavior;
};

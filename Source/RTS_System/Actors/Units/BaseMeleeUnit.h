// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Unit.h"
#include "TimerManager.h"
#include "Equipment/BaseWeapon.h"
#include "BaseMeleeUnit.generated.h"

/**
 * 
 */


class UMeleeAnimInstance;
class ABaseEquipment;
class UArmStatComponent;
class USkillObject;
class USkillAnimHandler;


enum class UNIT_BEHAVIOR {
	NOTHING,
	MOVABLE,
	FOLLOW_TARGET,
	BASICATTACK_ORDER,
	ATTACK_AVAILABLE,
	ATTACKING,
	SKILL_TARGETING,
	SKILL_ACTIVE_ORDER,
	SKILL_ACTIVE,
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

	void FollowTarget();
	void StopMovement();
	void BasicAttack();
	void FaceTarget();


	/* Skill system. */
	void SkillActivator();
	void SkillAttackCheck();


	/* Behavior controller. */
	bool CheckBehavior(UNIT_BEHAVIOR var);
	void TurnOnBehavior(UNIT_BEHAVIOR var);
	void TurnOffBehavior(UNIT_BEHAVIOR var);
	
	UFUNCTION(BlueprintCallable)
		void SkillTargetingFinish();
	UFUNCTION(BlueprintCallable)
		void TurnOffSkillRange();

	UFUNCTION()
		void DestroyCaller();
	UFUNCTION()
		void SetBasicAnimInstance();

	void EquipmentMount(ABaseEquipment* Item);


	UPROPERTY(EditAnywhere) 
		UMeleeAnimInstance* DefaultAnimInstance;
	UPROPERTY()
		UArmStatComponent* ArmStatComponent;
	UPROPERTY(EditAnywhere)
		UDecalComponent* DecalSkillRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDecalComponent* SkillAreaRange;

	FTimerHandle AttackSpeedTimer;
	FTimerHandle DestroyTimer;

	float skillRadius;
	bool bGoBasicAnimInstance = false;
	bool bShowSkillArea = false;

	// debug.
	void Debug_FogVision();

private:
	// Get target info from mainHUD.
	void AfterAttack();
	void AttackAvailable();

	UFUNCTION()
		void DeadProcess();

	FCriticalSection _target_mutex, _attack_mutex, _behavior_mutex;
	UNIT_BEHAVIOR Behavior;
};

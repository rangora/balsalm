// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Unit.generated.h"


class UFogRegister;
class USkillObject;
class UStatComponent;
class UAStarComponent;
class UDecoComponent;
class UHeadUpHPbar;
class UAnimInstance;
class ABaseWeapon;



UCLASS()
class RTS_SYSTEM_API AUnit : public ACharacter {
	GENERATED_BODY()

public:
	AUnit();
	virtual void BeginPlay() override;
	virtual float TakeDamage(float damageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* Causer) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Interaction(const FVector& RB_Vector, const AActor* Target);
	virtual void Interaction_Implementation(const FVector& RB_Vector, AActor* Target);

	virtual void AttackCheck();

	// For external bluerpint.
	UFUNCTION(BlueprintCallable)
		void SetTargetUnit(AUnit* pUnit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int unit_Team_Number;

	UPROPERTY()
		AUnit* TargetUnit;

	UPROPERTY()
		USkillObject* SkillRef = nullptr;

	ABaseWeapon* Weapon;

	// Variables.
	UPROPERTY()
		USphereComponent* AttackRadius;

	// Decoration.
	UPROPERTY(VisibleAnywhere, Category = UI)
		UWidgetComponent* HeadUpHPbar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* SelectionMesh;

	// Components.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Decoration)
		UDecoComponent* Deco;
	UPROPERTY(VisibleAnywhere, Category = Stat)
		UStatComponent* UnitStat;
	
	// External module.
	UPROPERTY()
		UAStarComponent* Astar;
	UPROPERTY()
		UFogRegister* FogRegister = nullptr;
};

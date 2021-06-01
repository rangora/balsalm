// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillAnimHandler.h"
#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"
#include "Templates/SubclassOf.h"
#include "Axe_CycloneAxe.generated.h"

/**
 * 
 */


class UAxeSkillAnimInstance;


UCLASS()
class RTS_SYSTEM_API UAxe_CycloneAxe : public USkillAnimHandler {
	GENERATED_BODY()
public:
	UAxe_CycloneAxe();

	/* A parameter, pUnit caller these functions. */
	virtual void ActiveSkill(AUnit* pUnit) override;
	virtual void ShowSkillArea(AUnit* pUnit, FVector CursorLocation) override;
	
	virtual void PlaySkillAnimation(AUnit* pUnit) override;
	virtual void AreaSkillJudge(AUnit* pUnit) override;


	float GetAngle(FVector& StartTrace, FVector& EndTrace);
	int GetQuardrant(FVector& vec);



	UPROPERTY()
		UMaterialInterface* AreaMaterial;
	UPROPERTY()
		UDecalComponent* SkillAreaDecal;
	UPROPERTY()
		UAxeSkillAnimInstance* AxeSkillAnimInstance;
	UPROPERTY()
		TSubclassOf<UAnimInstance> AxeSkillAnimInstanceClass;

private:
	FCriticalSection _attack_mutex;
	FVector MouseLocation;
	bool bDrawDebug = false;
	float _offset = 0.f;
	float _degree = 0.f;
	size_t segment;
};

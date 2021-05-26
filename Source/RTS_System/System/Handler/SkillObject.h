// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../../ActorType.h"
#include "SkillObject.generated.h"

/**
 * 
 */

struct SkillVariable;
class UAnimInstance;
class UCoolDownHandler;
class USkillAnimHandler;
class AUnit;
class ABaseMeleeUnit;



DECLARE_DELEGATE_OneParam(FSkillCoolTimeAction, bool);


UCLASS()
class RTS_SYSTEM_API USkillObject : public UObject {
	GENERATED_BODY()
public:
	USkillObject();
	void Init();
	void Clone(USkillObject* SrcObject);
	void Clear();

	/* SkillDataHandler. */
	void SetSkillAnimMgr(USkillAnimHandler* pSkillAnimMgr);
	void SetSkillParams(SkillVariable* pSkillVariable);


	/* Functions are binding with SkillAnimMgr. */
	void SkillActivator(AUnit* pUnit);
	void CheckSkillRange(AUnit* pUnit);
	
	UFUNCTION(BlueprintCallable)
		void ShowSkillArea(AUnit* pUnit, FVector CursorLocation);
	
	UFUNCTION(BlueprintCallable)
		void SkillAction(AUnit* pUnit);
	
	void AreaSkillJudge(AUnit* pUnit);

	/* Internerally called in skill action function. */
	UFUNCTION(BlueprintCallable)
		void PlaySkillAnimation(AUnit* pUnit);

	FName GetID();
	SkillVariable* GetSkillParams();
	UTexture2D* GetThumbnailTexture();


	/* Functions are binding with CoolDownMgr. */
	UFUNCTION(BlueprintCallable)
		float GetCoolDownMax();
	UFUNCTION(BlueprintCallable)
		float GetCurrentCoolDown();
	bool bCoolDown();


	FSkillCoolTimeAction SkillCoolTimeAction;


private:
	UPROPERTY(BlueprintReadwrite, meta = (AllowPrivateAccess = "true"))
		UCoolDownHandler* CoolDownMgr = nullptr;
	UPROPERTY()
		USkillAnimHandler* SkillAnimMgr = nullptr;
};

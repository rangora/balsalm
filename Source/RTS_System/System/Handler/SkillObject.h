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





UCLASS()
class RTS_SYSTEM_API USkillObject : public UObject {
	GENERATED_BODY()
public:
	USkillObject();
	void Init();
	void Clone(USkillObject* SrcObject);


	/* Interaction functions for external modules. */
	FName GetID();
	UTexture2D* GetThumbnailTexture();


	/* Functions are used for SkillAnimMgr. */
	virtual void CheckSkillRange(AUnit* pUnit);
	virtual void ActiveSkill(AUnit* pUnit);
	virtual void SkillAction(AUnit* pUnit);


	/* Values are used for CoolDownMgr. */
	UFUNCTION(BlueprintCallable)
		float GetCoolDownMax();
	UFUNCTION(BlueprintCallable)
		float GetCurrentCoolDown();

	UPROPERTY(BlueprintReadwrite)
		UCoolDownHandler* CoolDownMgr = nullptr;
	UPROPERTY()
		USkillAnimHandler* SkillAnimMgr = nullptr;
};

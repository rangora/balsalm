// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "UObject/NoExportTypes.h"
#include "CoolDownHandler.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API UCoolDownHandler : public UObject, public FTickableGameObject {
	GENERATED_BODY()
public:
	UCoolDownHandler();
	virtual void PostInitProperties() override;
	virtual void Tick(float delta) override;
	virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable)
		void CoolDownTrigger(bool param_bCoolDown);

	UFUNCTION(BlueprintCallable)
		bool IsCoolDown();

	float currentCoolDown;
	float coolDownMax;

private:
	bool bCoolDown = false;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "CoolDownHandler.h"
#include "SkillAnimHandler.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"


UCoolDownHandler::UCoolDownHandler() {
	coolDownMax = currentCoolDown = 0.f;
}

void UCoolDownHandler::Tick(float delta) {
	if (bCoolDown) {
		currentCoolDown = UKismetMathLibrary::FInterpTo_Constant(currentCoolDown, coolDownMax, delta, 1.0f);
		if (currentCoolDown >= coolDownMax) {
			bCoolDown = false;
			currentCoolDown = 0.f;
		}
	}
}

TStatId UCoolDownHandler::GetStatId() const {
	return UObject::GetStatID();
}

void UCoolDownHandler::CoolDownTrigger(bool param_bCoolDown) {
	bCoolDown = param_bCoolDown;
}

bool UCoolDownHandler::IsCoolDown() {
	return bCoolDown;
}

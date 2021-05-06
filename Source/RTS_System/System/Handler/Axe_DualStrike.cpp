// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe_DualStrike.h"
#include "Engine/Engine.h"


UAxe_DualStrike::UAxe_DualStrike() {
	Skill_ID = "2";
}

void UAxe_DualStrike::ActiveSkill(AUnit* pUnit) {
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Green, TEXT("UAxe_DualStrike!!"));
}

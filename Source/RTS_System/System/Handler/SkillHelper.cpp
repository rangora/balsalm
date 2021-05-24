// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillHelper.h"
#include "DrawDebugHelpers.h"
#include "../../Actors/Units/BaseMeleeUnit.h"



void USkillHelper::ShowSkillArea(ABaseMeleeUnit* pUnit) {
	FVector StartTrace = pUnit->GetActorLocation();
	FVector EndTrace = pUnit->GetActorForwardVector() * 1000.f + StartTrace;

	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Green);
}


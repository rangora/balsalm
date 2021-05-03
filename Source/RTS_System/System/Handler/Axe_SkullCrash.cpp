// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe_SkullCrash.h"
#include "Engine/Texture2D.h"
#include "Engine/Engine.h"


UAxe_SkullCrash::UAxe_SkullCrash() {
	Skill_ID = "1";
}

void UAxe_SkullCrash::ActiveSkill() {
	GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Green, TEXT("UAxe_SkullCrash!!"));
}

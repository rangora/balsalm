// Fill out your copyright notice in the Description page of Project Settings.


#include "MainInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Handler/SkillDataHandler.h"
#include "../DataTable/ABaseSkillTable.h"

UMainInstance::UMainInstance() {
	
}

void UMainInstance::Init() {
	Super::Init();
	SkillDataHandler = NewObject<USkillDataHandler>();
}

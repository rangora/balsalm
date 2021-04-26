// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
//TEST
//#include "Handler/SkillDataHandler.h"
//#include "MainInstance.h"
//#include "../DataTable/ABaseSkillTable.h"
//

// Sets default values
ACameraPawn::ACameraPawn() {
	PrimaryActorTick.bCanEverTick = true;
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACameraPawn::BeginPlay() {
	Super::BeginPlay();

	
	//TEST
	//auto IInstance = Cast<UMainInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//BaseSkillData_Info* skillslot = new BaseSkillData_Info;
	//skillslot = (AttackSkillData_Info*)skillslot;
	//IInstance->SkillDataHandler->SetSkillData(SKILLTYPE::WEAPON_MELEE, 1, skillslot);
	//UE_LOG(LogTemp, Log, TEXT("OUt"));
	//UE_LOG(LogTemp, Log, TEXT("%s"),*skillslot->Description);
	//auto tslot = (AttackSkillData_Info*)skillslot;
	//UE_LOG(LogTemp, Log, TEXT("Range:%f"), tslot->AttackRange);
	//
}

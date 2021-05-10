// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"
#include "../../System/UnitController.h"
#include "../../System/MainController.h"
#include "../../System/MainGameMode.h"
#include "../../UI/HeadUpHPbar.h"
#include "../Misc/PathSphere.h"
#include "../Misc/AttackCaculator.h"
#include "Component/StatComponent.h"
#include "Component/AstarComponent.h"
#include "Component/DecoComponent.h"
#include "FogRegister.h"
#include "FogManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AUnit::AUnit() 
	: unit_Team_Number(1),
	TargetUnit(nullptr) {
	PrimaryActorTick.bCanEverTick = true;

	SelectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionMesh"));
	SelectionMesh->SetupAttachment(GetMesh());

	HeadUpHPbar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadUpHPbar"));
	HeadUpHPbar->SetupAttachment(GetMesh());

	AttackRadius = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRadius"));
	AttackRadius->SetupAttachment(GetMesh());

	UnitStat = CreateDefaultSubobject<UStatComponent>(TEXT("UnitStat"));
	Astar = CreateDefaultSubobject<UAStarComponent>(TEXT("Astar"));
	Deco = CreateDefaultSubobject<UDecoComponent>(TEXT("Decoration"));

	AIControllerClass = AUnitController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
	bUseControllerRotationYaw = false;

	FogRegister = CreateDefaultSubobject<UFogRegister>(TEXT("FogRegister"));
}

void AUnit::BeginPlay() {
	Super::BeginPlay();

	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (unit_Team_Number == IMode->player_Team_Number) {
		FogRegister->RegisterToManager();
	}
}

float AUnit::TakeDamage(float damageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* Causer) {
	float totalDamage = Super::TakeDamage(damageAmount, DamageEvent, EventInstigator, Causer);
	
	UnitStat->TakeDamage(totalDamage);

	// debug
	float am =UnitStat->currentHP;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, 
		FString::Printf(TEXT("am:%f"),am));

	return totalDamage;
}

void AUnit::Interaction_Implementation(const FVector& RB_Vector, AActor* Target) {}

void AUnit::AttackCheck() {
	if (IsValid(TargetUnit)) {
		float amount = UAttackCaculator::CaculateDamage(this, TargetUnit);
		FDamageEvent DamageEvent;
		TargetUnit->TakeDamage(amount, DamageEvent, GetController(), this);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("UNVALID own targetunit!!"));
	}
}

void AUnit::SetTargetUnit(AUnit* pUnit) {
	if (IsValid(pUnit))
		TargetUnit = pUnit;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"
#include "../../System/UnitController.h"
#include "../../System/MainController.h"
#include "../../System/MainGameMode.h"
#include "../../UI/HeadUpHPbar.h"
#include "../../ActorType.h"
#include "../Misc/PathSphere.h"
#include "../Misc/AttackCaculator.h"
#include "Equipment/BaseWeapon.h"
#include "Component/StatComponent.h"
#include "Component/AstarComponent.h"
#include "Component/DecoComponent.h"
#include "FogRegister.h"
#include "FogManager.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"






AUnit::AUnit() 
	: unit_Team_Number(1) {
	PrimaryActorTick.bCanEverTick = true;
	TargetUnits.Add(nullptr);

	SelectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionMesh"));
	SelectionMesh->SetupAttachment(GetMesh());

	HeadUpHPbar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HeadUpHPbar"));
	HeadUpHPbar->SetupAttachment(GetMesh());

	AttackRadius = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRadius"));
	PathFollower = CreateDefaultSubobject<USphereComponent>(TEXT("PathFollower"));
	AttackRadius->SetupAttachment(GetMesh());
	PathFollower->SetupAttachment(GetMesh());
	PathFollower->SetSphereRadius(5.f);

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
}

void AUnit::Tick(float delta) {
	Super::Tick(delta);

	int n = GazerUnit.Num();

	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	if(IMode->player_Team_Number != unit_Team_Number) {
		UnitVision();
	}
}

float AUnit::TakeDamage(float damageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* Causer) {
	float totalDamage = Super::TakeDamage(damageAmount, DamageEvent, EventInstigator, Causer);
	
	UnitStat->TakeDamage(totalDamage);
	return totalDamage;
}

void AUnit::Interaction_Implementation(const FVector& RB_Vector, AActor* Target) {}

void AUnit::AttackCheck() {
	auto TargetUnit = TargetUnits[0];
	if (IsValid(TargetUnit)) {
		float amount = UAttackCaculator::CaculateDamage(this, TargetUnit);
		FDamageEvent DamageEvent;
		TargetUnit->TakeDamage(amount, DamageEvent, GetController(), this);
	}
}

void AUnit::SetVisivility(bool bVisible) {
	GetMesh()->SetVisibility(bVisible);
	Weapon->SetVisibility(bVisible);
	HeadUpHPbar->SetVisibility(bVisible);
}

void AUnit::GetGazerUnit(AUnit* Gazer) {
	FScopeLock _lockGaurd(&_visionMutex);

	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!IsValid(Gazer) || Gazer->IsPendingKill()) return;
	if (IMode->player_Team_Number == unit_Team_Number) return;
	if (Gazer->UnitStat->DeadOrAlive == DOA::DEAD) return;

	if (!GazerUnit.Contains(Gazer)) {
		GazerUnit.Add(Gazer);
	}
}

void AUnit::UnitVision() {
	if (_visionMutex.TryLock()) {
		FCollisionQueryParams LayTraceParams(FName(TEXT("EnemyVision Params")), false, this);
		FHitResult Results;
		TSet<AUnit*> RemoveUnit;
		bool bHit;

		for (auto Element : GazerUnit) {
			bool bRemove = false;

			bHit = GetWorld()->LineTraceSingleByChannel(Results, GetActorLocation(), Element->GetActorLocation(),
				ECollisionChannel::ECC_WorldStatic, LayTraceParams);


			if (Results.Distance > Element->GetSightRange()) {
				bRemove = true;
			}
			else if (Element->UnitStat->DeadOrAlive == DOA::DEAD) {
				bRemove = true;
			}
			else if (bHit) {
				auto TActor = Results.GetActor();
				if (!TActor->IsA(AUnit::StaticClass())) {
					bRemove = true;
				}
			}

			if (bRemove) 
				RemoveUnit.Add(Element);
		}
		
		for (auto Element : RemoveUnit) {
			GazerUnit.Remove(Element);
		}
		
		if (GazerUnit.Num()) {
			SetVisivility(true);
		}
		else SetVisivility(false);

		_visionMutex.Unlock();
	}
}

float AUnit::GetSightRange() {
	return SightRange * FogRegister->Manager->TexelUnit;
}


void AUnit::SetTargetUnit(AUnit* pUnit) {
	if (IsValid(pUnit)) 
		if(pUnit->UnitStat->DeadOrAlive == DOA::ALIVE)
			TargetUnits[0] = pUnit;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeUnit.h"
#include "Component/StatComponent.h"
#include "Component/AstarComponent.h"
#include "Component/ArmStatComponent.h"
#include "Animation/MeleeAnimInstance.h"
#include "../../ActorType.h"
#include "../../System/MainGameMode.h"
#include "../../System/Handler/SkillObject.h"
//#include "../../UI/SkillControlUI.h"


ABaseMeleeUnit::ABaseMeleeUnit() : bReadyBasicAttack(false) {
	ArmStatComponent = NewObject<UArmStatComponent>();

	for (int i = 0; i < UNIT_SKILLSLOT_LENGTH; i++)
		ArmStatComponent->SkillArray.Add(NewObject<USkillObject>());
}

void ABaseMeleeUnit::Tick(float delta) {
	Super::Tick(delta);

	if (bReadyBasicAttack) BasicAttack();
}

void ABaseMeleeUnit::BeginPlay() {
	Super::BeginPlay();

	FName WeaponSocket(TEXT("R_Hand_Socket"));
	FString SampleWeaponPath = "/Game/Blueprints/Actors/Equipment/BP_SampleAxe.BP_SampleAxe_C";
	UClass* WeaponClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *SampleWeaponPath));
	
	auto SampleWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (IsValid(SampleWeapon)) {
		SampleWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		Weapon = SampleWeapon;
		EquipmentMount(Weapon);
	}
}

void ABaseMeleeUnit::PostInitializeComponents() {
	Super::PostInitializeComponents();

	AnimInstance = Cast<UMeleeAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->DeleFunc_AttackCheck.AddUObject(this, &AUnit::AttackCheck);
}

void ABaseMeleeUnit::Interaction_Implementation(const FVector& RB_Vector, AActor* Target) {
	Super::Interaction_Implementation(RB_Vector, Target);

	bReadyBasicAttack = false;

	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IMode == nullptr || (IMode->player_Team_Number != unit_Team_Number)) return;
	
	auto TUnit = Cast<AUnit>(Target);

	if (AnimInstance->IsAnyMontagePlaying()) 
		AnimInstance->StopAllMontages(0.5f);

	Astar->MoveToLocation(RB_Vector);

	if (!IsValid(TUnit)) return;

	if (TUnit->IsPendingKill() || (TUnit->UnitStat->DeadOrAlive == DOA::DEAD)) return;

	if (IMode->player_Team_Number != TUnit->unit_Team_Number) {
		TargetUnit = TUnit;
		bReadyBasicAttack = true;
	}
}

void ABaseMeleeUnit::BasicAttack() {
	FHitResult RayCastingResult;
	FVector TargetLocation;

	_mutex.Lock();
	if (TargetUnit->UnitStat->DeadOrAlive == DOA::ALIVE) {
		TargetLocation = TargetUnit->GetActorLocation();
	}
	_mutex.Unlock();

	float distance = FVector::Distance(TargetLocation, GetActorLocation());
	UE_LOG(LogTemp, Log, TEXT("distance:%f"), distance);

	if (distance > 0 && distance <= UnitStat->attackRange) {
		AnimInstance->PlayBasicAttack();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Attack!")));
		bReadyBasicAttack = false;
	}
}

void ABaseMeleeUnit::BasicAttackEnd() {}

void ABaseMeleeUnit::EquipmentMount(ABaseEquipment* Item) {
	switch (Item->ItemType) {
	case ITEMTYPE::WEAPON: {
		auto IWeapon = Cast<ABaseWeapon>(Item);

		if (IsValid(IWeapon)) {
			UnitStat->currentDamage += IWeapon->WeaponDamage;
			UnitStat->attackRange = IWeapon->WeaponRange;
			UnitStat->attackSpeed = IWeapon->WeaponSpeed;
			Weapon->WeaponType = IWeapon->WeaponType;
		}
		break;
	}
	
	case ITEMTYPE::ARMOR: 
		break;
	
	default:
		break;
	}
}

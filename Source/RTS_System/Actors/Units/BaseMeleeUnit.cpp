// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeUnit.h"
#include "Component/StatComponent.h"
#include "Component/AstarComponent.h"
#include "Component/ArmStatComponent.h"
#include "Animation/MeleeAnimInstance.h"
#include "../../ActorType.h"
#include "../../System/MainGameMode.h"
#include "../../System/Handler/SkillObject.h"



ABaseMeleeUnit::ABaseMeleeUnit() : bReadyBasicAttack(false) {
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_Basic(
		TEXT("/Game/Mannequin/Animations/ANIM_UnitGraph.ANIM_UnitGraph_C"));
	
	// Skill range dacal init. (test)
	DecalSkillRange = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalSkillRange"));
	DecalSkillRange->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_SkillRangeDecal(
		TEXT("/Game/Resources/Circle/MAT_DecalCircle.MAT_DecalCircle"));

	if (MAT_SkillRangeDecal.Succeeded()) {
		UMaterialInterface* RangeMaterial = MAT_SkillRangeDecal.Object;
		DecalSkillRange->SetDecalMaterial(RangeMaterial);
	}

	if (ANIM_Basic.Succeeded())
		BasicUnitAnimInstanceClass = ANIM_Basic.Class;
}

void ABaseMeleeUnit::Tick(float delta) {
	Super::Tick(delta);

	if (bReadyBasicAttack) BasicAttack();
	if (bGoBasicAnimInstance) SetBasicAnimInstance();
}

void ABaseMeleeUnit::BeginPlay() {
	Super::BeginPlay();

	// ArmStatComponent init.
	this->ArmStatComponent = NewObject<UArmStatComponent>();

	for (int i = 0; i < UNIT_SKILLSLOT_LENGTH; i++)
		this->ArmStatComponent->SkillArray.Add(NewObject<USkillObject>());

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

	// Check whether unit is a ally or not.
	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IMode == nullptr || (IMode->player_Team_Number != unit_Team_Number)) return;
	
	bReadyBasicAttack = false;

	auto DesiredTargetUnit = Cast<AUnit>(Target);

	// Stop current animation.
	if (AnimInstance->IsAnyMontagePlaying())
		AnimInstance->StopAllMontages(0.35f);

	Astar->MoveToLocation(RB_Vector);

	if (!IsValid(DesiredTargetUnit)) return;

	if (DesiredTargetUnit->IsPendingKill() || (DesiredTargetUnit->UnitStat->DeadOrAlive == DOA::DEAD)) return;

	if (IMode->player_Team_Number != DesiredTargetUnit->unit_Team_Number) {
		TargetUnit = DesiredTargetUnit;
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
	
	if (distance > 0 && distance <= UnitStat->attackRange) {
		AnimInstance->PlayBasicAttack();
		bReadyBasicAttack = false;
	}
}

void ABaseMeleeUnit::BasicAttackEnd() {}

void ABaseMeleeUnit::SetBasicAnimInstance() {
	auto CurrentAnimInstance = GetMesh()->GetAnimInstance();

	CurrentAnimInstance->StopAllMontages(0.f);
	
	const bool bBlockOnTask = true;
	const bool bPerformPostAnimEvaluation = true;

	GetMesh()->HandleExistingParallelEvaluationTask(bBlockOnTask, bPerformPostAnimEvaluation);
	
	GetMesh()->SetAnimInstanceClass(BasicUnitAnimInstanceClass);
	AnimInstance = Cast<UMeleeAnimInstance>(GetMesh()->GetAnimInstance());
	bGoBasicAnimInstance = false;
}

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

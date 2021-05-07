// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeUnit.h"
#include "Component/StatComponent.h"
#include "Component/AstarComponent.h"
#include "Component/ArmStatComponent.h"
#include "Animation/MeleeAnimInstance.h"
#include "../../ActorType.h"
#include "../../System/MainGameMode.h"
#include "../../System/Handler/SkillObject.h"



ABaseMeleeUnit::ABaseMeleeUnit() : skillRadius(0.f) {
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM_Basic(
		TEXT("/Game/Mannequin/Animations/ANIM_UnitGraph.ANIM_UnitGraph_C"));
	
	// Skill range dacal init.
	DecalSkillRange = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalSkillRange"));
	DecalSkillRange->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_SkillRangeDecal(
		TEXT("/Game/Resources/Circle/MAT_SkillDecalCircle.MAT_SkillDecalCircle"));

	if (MAT_SkillRangeDecal.Succeeded()) {
		UMaterialInterface* RangeMaterial = MAT_SkillRangeDecal.Object;
		DecalSkillRange->SetDecalMaterial(RangeMaterial);
	}

	// Skill radius sphere init.
	SkillRadius = CreateDefaultSubobject<USphereComponent>(TEXT("SkillRadius"));
	SkillRadius->SetupAttachment(GetMesh());

	// Basic anim init.
	if (ANIM_Basic.Succeeded())
		BasicUnitAnimInstanceClass = ANIM_Basic.Class;

	Behavior = UNIT_BEHAVIOR::MOVABLE;
}

void ABaseMeleeUnit::Tick(float delta) {
	Super::Tick(delta);

	if (CheckBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER)) BasicAttack();
	if (bGoBasicAnimInstance) SetBasicAnimInstance();
	if (bForSkill) SkillActivator();
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
	
	if (CheckBehavior(UNIT_BEHAVIOR::MOVABLE)) {

		// Cancel skill targeting process.
		if (CheckBehavior(UNIT_BEHAVIOR::SKILL_TARGETING)) {
			TurnOffBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);
			DecalSkillRange->SetVisibility(false);
		}

		// Cancel basic attack animation for movement.
		if (CheckBehavior(UNIT_BEHAVIOR::ATTACKING)) {
			AnimInstance->StopAllMontages(0.3f);
			TurnOffBehavior(UNIT_BEHAVIOR::ATTACKING);
		}
		
		// Move to location.
		Astar->MoveToLocation(RB_Vector);
			

		// Check wheater the clicked target is a unit or not.
		if (Target->IsA(AUnit::StaticClass())) {

			// Check the validation of unit.
			auto DesiredTargetUnit = Cast<AUnit>(Target);
			if (DesiredTargetUnit->IsPendingKill()
				|| (DesiredTargetUnit->UnitStat->DeadOrAlive == DOA::DEAD)) return;

			// If the unit is a foe, set as target.
			if (IMode->player_Team_Number != DesiredTargetUnit->unit_Team_Number) {
				TargetUnit = DesiredTargetUnit;
				TurnOnBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER);
			}
		}

		// Just movent order. Turn off other flag.
		else {
			TurnOffBehavior(UNIT_BEHAVIOR::ATTACKING);
			TurnOffBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER);
		}
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
		TurnOffBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER);
		TurnOnBehavior(UNIT_BEHAVIOR::ATTACKING);
	}
}

void ABaseMeleeUnit::BasicAttackEnd() {}

void ABaseMeleeUnit::AppointTheSkillTarget(float skillRange, USkillObject* ActivatedSkill) {
	// Show skill range.	
	auto NewSkillSize = DecalSkillRange->DecalSize;
	NewSkillSize.Y = skillRange;
	NewSkillSize.Z = skillRange;
	DecalSkillRange->DecalSize = NewSkillSize;

	skillRadius = skillRange;
	SkillRef = ActivatedSkill;
	DecalSkillRange->SetVisibility(true);
	TurnOnBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);
}

void ABaseMeleeUnit::SkillActivator() {
	FHitResult RayCastingResult;
	FVector TargetLocation;

	_mutex.Lock();
	if (TargetUnit->UnitStat->DeadOrAlive == DOA::ALIVE) {
		TargetLocation = TargetUnit->GetActorLocation();
	}
	_mutex.Unlock();

	float distance = FVector::Distance(TargetLocation, GetActorLocation());

	if (distance > 0 && distance <= DecalSkillRange->DecalSize.Y) {
		if (IsValid(SkillRef)) {
			SkillRef->SkillAction(this);
			AnimInstance->PlayBasicAttack();
			bForSkill = false;
		}
	}
}

void ABaseMeleeUnit::SetDefaultBehavior() {
}

bool ABaseMeleeUnit::CheckBehavior(UNIT_BEHAVIOR var) {
	int32 flag = static_cast<int32>(var);
	return static_cast<int32>(Behavior) & (1 << (flag - 1));
}

void ABaseMeleeUnit::TurnOnBehavior(UNIT_BEHAVIOR var) {
	int32 flag = static_cast<int32>(var);
	int32 newBehavior = static_cast<int32>(Behavior) | (1 << (flag - 1));
	Behavior = static_cast<UNIT_BEHAVIOR>(newBehavior);
}

void ABaseMeleeUnit::TurnOffBehavior(UNIT_BEHAVIOR var) {
	int32 flag = static_cast<int32>(var);
	int32 newBehavior = static_cast<int32>(Behavior) & (~(1 << (flag - 1)));
	Behavior = static_cast<UNIT_BEHAVIOR>(newBehavior);
}

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

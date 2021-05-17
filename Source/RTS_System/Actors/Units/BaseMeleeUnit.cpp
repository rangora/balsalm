// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeUnit.h"
#include "Kismet/KismetMathLibrary.h"
#include "Component/StatComponent.h"
#include "Component/AstarComponent.h"
#include "Component/ArmStatComponent.h"
#include "Component/DecoComponent.h"
#include "Equipment/BaseWeapon.h"
#include "Animation/MeleeAnimInstance.h"
#include "../../ActorType.h"
#include "../../System/MainGameMode.h"
#include "../../System/MainController.h"
#include "../../System/Handler/SkillObject.h"
#include "../../System/Handler/SkillAnimHandler.h"
#include "../../UI/MainHUD.h"
#include "../../UI/ScreenUI.h"
#include "../Misc/AttackCaculator.h"



/* In-build functions. */
ABaseMeleeUnit::ABaseMeleeUnit() : skillRadius(0.f) {
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

	Behavior = UNIT_BEHAVIOR::MOVABLE;
	TurnOnBehavior(UNIT_BEHAVIOR::ATTACK_AVAILABLE);
}

void ABaseMeleeUnit::Tick(float delta) {
	Super::Tick(delta);

	if (CheckBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER)) BasicAttack();
	if (bGoBasicAnimInstance) SetBasicAnimInstance();
	if (CheckBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE_ORDER)) SkillActivator();
}

void ABaseMeleeUnit::BeginPlay() {
	Super::BeginPlay();

	// ArmStatComponent init.
	this->ArmStatComponent = NewObject<UArmStatComponent>();

	for (int i = 0; i < UNIT_SKILLSLOT_LENGTH; i++)
		this->ArmStatComponent->SkillArray.Add(NewObject<USkillObject>());

	FName WeaponSocket(TEXT("R_Hand_Socket"));
	FString SampleWeaponPath = "/Game/Blueprints/Actors/Equipment/BP_SampleWeapon.BP_SampleWeapon_C";
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

	DefaultAnimInstance = Cast<UMeleeAnimInstance>(GetMesh()->GetAnimInstance());

	UnitStat->AfterDeadProcess.BindUFunction(this, FName("DeadProcess"));
}




/* User functions. */
void ABaseMeleeUnit::Interaction_Implementation(const FVector& RB_Vector, AActor* Target) {
	Super::Interaction_Implementation(RB_Vector, Target);

	// Check whether unit is a ally or not.
	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IMode == nullptr || (IMode->player_Team_Number != unit_Team_Number)) return;
	
	if (CheckBehavior(UNIT_BEHAVIOR::MOVABLE)) {

		// Cancel skill targeting process.
		if (CheckBehavior(UNIT_BEHAVIOR::SKILL_TARGETING)) {
			_behavior_mutex.Lock();
			TurnOffBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);
			_behavior_mutex.Unlock();
			ShowSkillRadius(false);
		}

		// Check wheater the clicked target is a unit or not.
		if (Target->IsA(AUnit::StaticClass())) {

			// Check the validation of unit.
			auto DesiredTargetUnit = Cast<AUnit>(Target);
			if (DesiredTargetUnit->IsPendingKill()
				|| (DesiredTargetUnit->UnitStat->DeadOrAlive == DOA::DEAD)) return;

			// If the unit is a foe, set as target.
			if (IMode->player_Team_Number != DesiredTargetUnit->unit_Team_Number) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("unit"));


				if (TargetUnit != DesiredTargetUnit) {
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("diff"));
			
					TargetUnit = DesiredTargetUnit;
					TurnOnBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER);
					Astar->MoveToLocation(RB_Vector);
				}
			}
		}

		//  Movement order. Turn off other flag.
		else {
			TargetUnit = nullptr;
			if(CheckBehavior(UNIT_BEHAVIOR::ATTACKING))
				DefaultAnimInstance->StopAllMontages(0.3f);
			
			TurnOffBehavior(UNIT_BEHAVIOR::ATTACKING);
			TurnOffBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER);

			// Move to location.
			Astar->MoveToLocation(RB_Vector);
		}
	}
}

void ABaseMeleeUnit::StopMovement() {
	GetCharacterMovement()->StopMovementImmediately();
	Astar->ClearRoute();
}

void ABaseMeleeUnit::BasicAttack() {
	FHitResult RayCastingResult;
	FVector TargetLocation;

	if (TargetUnit->UnitStat->DeadOrAlive == DOA::ALIVE) {
		TargetLocation = TargetUnit->GetActorLocation();
	}
	else {
		TargetUnit = nullptr;
		TurnOffBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER);
		return;
	}

	float distance = FVector::Distance(TargetLocation, GetActorLocation());
	
	if (distance >= 0 && distance <= UnitStat->attackRange) {
		if (CheckBehavior(UNIT_BEHAVIOR::ATTACK_AVAILABLE)) {
			StopMovement();
			FaceTarget();
			DefaultAnimInstance->PlayBasicAttack();
			AfterAttack();
		}
	}
}

void ABaseMeleeUnit::FaceTarget() {
	if (IsValid(TargetUnit)) {
		auto DesiredRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetUnit->GetActorLocation());
		SetActorRotation(DesiredRotation);
	}
}

void ABaseMeleeUnit::AppointTheSkillTarget(float skillRange) {
	// Show skill range.	
	auto NewSkillSize = DecalSkillRange->DecalSize;
	NewSkillSize.Y = skillRange;
	NewSkillSize.Z = skillRange;
	DecalSkillRange->DecalSize = NewSkillSize;

	skillRadius = skillRange;
	ShowSkillRadius(true);
	RequiredTargeting();

	TurnOnBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);
}

void ABaseMeleeUnit::SkillActivator() {
	if (!IsValid(TargetUnit)) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("non-SkillActivator"));
		return;
	}

	float distance = FVector::Distance(TargetUnit->GetActorLocation(), GetActorLocation());

	if (distance >= 0 && distance <= DecalSkillRange->DecalSize.Y) {
		if (IsValid(SkillRef)) {
			StopMovement();
			FaceTarget();
			SkillRef->SkillAction(this);

			_behavior_mutex.Lock();
			TurnOffBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE_ORDER);
			TurnOnBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE);
			_behavior_mutex.Unlock();
		}
	}
}

void ABaseMeleeUnit::SkillAttackCheck() {
	if (IsValid(TargetUnit)) {
		float amount = UAttackCaculator::SkillDamage(this, TargetUnit);
		FDamageEvent DamageEvent;
		TargetUnit->TakeDamage(amount, DamageEvent, GetController(), this);
	}
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

void ABaseMeleeUnit::ShowSkillRadius(bool bShow) {
	DecalSkillRange->SetVisibility(bShow);
}

void ABaseMeleeUnit::SkillTargetingFinish() {
	TurnOffBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);

	// Can't be target is myself.
	if (TargetUnit == this) return;
	
	TurnOnBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE_ORDER);
	

	// Move to location.
	FVector TargetLocation;
	_mutex.Lock();
	if (TargetUnit->UnitStat->DeadOrAlive == DOA::ALIVE) {
		TargetLocation = TargetUnit->GetActorLocation();
	}
	_mutex.Unlock();

	Astar->MoveToLocation(TargetLocation);
}

void ABaseMeleeUnit::SetBasicAnimInstance() {
	auto CurrentAnimInstance = GetMesh()->GetAnimInstance();

	CurrentAnimInstance->StopAllMontages(0.f);
	
	const bool bBlockOnTask = true;
	const bool bPerformPostAnimEvaluation = true;

	GetMesh()->HandleExistingParallelEvaluationTask(bBlockOnTask, bPerformPostAnimEvaluation);	
	
	
	GetMesh()->SetAnimInstanceClass(DefaultAnimInstance->GetClass());
	DefaultAnimInstance = Cast<UMeleeAnimInstance>(GetMesh()->GetAnimInstance());
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





/* Private functions. */
void ABaseMeleeUnit::RequiredTargeting() {
	auto IController = Cast<AMainController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	auto IHUD = Cast<AMainHUD>(IController->GetHUD());

	if (IsValid(IHUD)) {
		IHUD->MouseLeftButtonActionSwitcher();
	}
}

void ABaseMeleeUnit::AfterAttack() {
	float attackSpeedRate = UnitStat->attackSpeed;

	TurnOnBehavior(UNIT_BEHAVIOR::ATTACKING);
	TurnOffBehavior(UNIT_BEHAVIOR::ATTACK_AVAILABLE);

	GetWorldTimerManager().SetTimer(AttackSpeedTimer, this, &ABaseMeleeUnit::AttackAvailable, attackSpeedRate, false);
}

void ABaseMeleeUnit::AttackAvailable() {
	while (true) {
		bool bGet = _behavior_mutex.TryLock();
		if (bGet) {
			TurnOnBehavior(UNIT_BEHAVIOR::ATTACK_AVAILABLE);
			_behavior_mutex.Unlock();
			break;
		}
	}

	GetWorldTimerManager().ClearTimer(AttackSpeedTimer);
}

void ABaseMeleeUnit::DeadProcess() {
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	bGoBasicAnimInstance = true;
	DefaultAnimInstance->bDead = true;

	_behavior_mutex.Lock();
	Behavior = UNIT_BEHAVIOR::NOTHING;
	_behavior_mutex.Unlock();

	// Set docoes unvisibility.
	Deco->SelectionMeshRef->SetVisibility(false);
	Deco->HeadUpHPbarRef->SetVisibility(false);


	// Clear skill HUD if selected a ally unit.
	auto IController = Cast<AMainController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(IController)) {
		if (IController->IsOnlyOneAllyUnitSelected()) {
			
			if (IController->Units[0] == this) {
				IController->SetSkillPanelVisibility(false);
			}
		}
	}
}

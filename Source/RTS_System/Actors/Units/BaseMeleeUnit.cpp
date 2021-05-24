// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeUnit.h"
#include "Engine/Engine.h"
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
	SkillAreaRange = CreateDefaultSubobject<UDecalComponent>(TEXT("SkillAreaRange"));
	DecalSkillRange->SetupAttachment(GetMesh());
	SkillAreaRange->SetupAttachment(GetMesh());


	static ConstructorHelpers::FObjectFinder<UMaterial> MAT_SkillRangeDecal(
		TEXT("/Game/Resources/Circle/MAT_SkillDecalCircle.MAT_SkillDecalCircle"));

	if (MAT_SkillRangeDecal.Succeeded()) {
		UMaterialInterface* RangeMaterial = MAT_SkillRangeDecal.Object;
		DecalSkillRange->SetDecalMaterial(RangeMaterial);
	}

	Behavior = UNIT_BEHAVIOR::MOVABLE;
	TurnOnBehavior(UNIT_BEHAVIOR::ATTACK_AVAILABLE);
}

void ABaseMeleeUnit::Tick(float delta) {
	Super::Tick(delta);

	if (CheckBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER)) BasicAttack();
	if (CheckBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE_ORDER)) SkillActivator();
	if (bGoBasicAnimInstance) SetBasicAnimInstance();
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
	

	// Check wheater the clicked target is a unit or not.
	if (Target->IsA(AUnit::StaticClass())) {

		// Check the validation of unit.
		auto DesiredTargetUnit = Cast<AUnit>(Target);
		if (DesiredTargetUnit->IsPendingKill()
			|| (DesiredTargetUnit->UnitStat->DeadOrAlive == DOA::DEAD)) return;

		// If the unit is a foe, set as target.
		if (IMode->player_Team_Number == DesiredTargetUnit->unit_Team_Number) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("unit"));


			if (TargetUnits[0] != DesiredTargetUnit) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("diff"));
			
				TargetUnits[0] = DesiredTargetUnit;
				TurnOnBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER);
				Astar->MoveToLocation(RB_Vector);
			}
		}
	}

	//  Clicked somewhere is located place.
	else {
		// Skill can't be canceled.
		if (CheckBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE)) return;

		_mutex.Lock();
		TargetUnits[0] = nullptr;
		_mutex.Unlock();

		// Cancel skill targeting process.
		if (CheckBehavior(UNIT_BEHAVIOR::SKILL_TARGETING)) {
			auto IController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			auto IHUD = Cast<AMainHUD>(IController->GetHUD());

			IHUD->SetMouseLeftButtonAction(LeftButtonAction::DRAG);
			TurnOffBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);
			TurnOffSkillRange();
		}
		DefaultAnimInstance->StopAllMontages(0.3f);
		
		TurnOnBehavior(UNIT_BEHAVIOR::MOVABLE);
		TurnOffBehavior(UNIT_BEHAVIOR::ATTACKING);
		TurnOffBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER);

		// Move to location.
		Astar->MoveToLocation(RB_Vector);
	}
}

void ABaseMeleeUnit::FollowTarget() {
	if (IsValid(TargetUnits[0]) && CheckBehavior(UNIT_BEHAVIOR::MOVABLE)) {
		Astar->MoveToLocation(TargetUnits[0]->GetActorLocation());
	}
	else StopMovement();
}

void ABaseMeleeUnit::StopMovement() {
	GetCharacterMovement()->StopMovementImmediately();
	Astar->ClearRoute();
}

void ABaseMeleeUnit::BasicAttack() {
	FHitResult RayCastingResult;
	FVector TargetLocation;

	_mutex.Lock();
	if (IsValid(TargetUnits[0])) {
		if (TargetUnits[0]->UnitStat->DeadOrAlive == DOA::ALIVE) {
			TargetLocation = TargetUnits[0]->GetActorLocation();
			_mutex.Unlock();
		}
		else {
			TargetUnits[0] = nullptr;
			TurnOffBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER);
			_mutex.Unlock();
			return;
		}
	}

	float distance = FMath::Abs(FVector::Distance(TargetLocation, GetActorLocation()));

	if (distance >= 0 && distance <= UnitStat->attackRange) {
		if (CheckBehavior(UNIT_BEHAVIOR::ATTACK_AVAILABLE)) {
			StopMovement();
			FaceTarget();
			DefaultAnimInstance->PlayBasicAttack();
			AfterAttack();
		}
	}
	else FollowTarget();
	
}

void ABaseMeleeUnit::FaceTarget() {
	if (IsValid(TargetUnits[0])) {
		auto DesiredRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetUnits[0]->GetActorLocation());
		SetActorRotation(DesiredRotation);
	}
}

void ABaseMeleeUnit::SkillActivator() {
	// Check target validation.
	if (!IsValid(TargetUnits[0])) return;

	float distance = FMath::Abs(FVector::Distance(TargetUnits[0]->GetActorLocation(), GetActorLocation()));

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
	else FollowTarget();
}

void ABaseMeleeUnit::SkillAttackCheck() {
	if (IsValid(TargetUnits[0])) {
		float amount = UAttackCaculator::SkillDamage(this, TargetUnits[0]);
		FDamageEvent DamageEvent;
		TargetUnits[0]->TakeDamage(amount, DamageEvent, GetController(), this);
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

void ABaseMeleeUnit::SkillTargetingFinish() {
	TurnOffBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);

	// Can't be target is myself.
	if (TargetUnits[0] == this) return;
	
	TurnOnBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE_ORDER);
	

	// Move to location.
	FVector TargetLocation;
	_mutex.Lock();
	if (TargetUnits[0]->UnitStat->DeadOrAlive == DOA::ALIVE) {
		TargetLocation = TargetUnits[0]->GetActorLocation();
	}
	_mutex.Unlock();

	Astar->MoveToLocation(TargetLocation);
}

void ABaseMeleeUnit::TurnOffSkillRange() {
	DecalSkillRange->SetVisibility(false);
	SkillAreaRange->SetVisibility(false);
	bShowSkillArea = false;
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
	StopMovement();
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeUnit.h"
#include "FogRegister.h"
#include "FogManager.h"
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

	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (IMode->player_Team_Number == unit_Team_Number) {
		//Debug_FogVision();
	}
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

	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (unit_Team_Number == IMode->player_Team_Number) {
		FogRegister->RegisterToManager();
		SetVisivility(true);
	}
	else {
		SetVisivility(false);
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
	//if (IMode == nullptr || (IMode->player_Team_Number != unit_Team_Number)) return;
	
	// Check wheater the clicked target is a unit or not.
	if (Target->IsA(AUnit::StaticClass())) {

		// Check the validation of unit.
		auto DesiredTargetUnit = Cast<AUnit>(Target);
		if (DesiredTargetUnit->IsPendingKill()
			|| (DesiredTargetUnit->UnitStat->DeadOrAlive == DOA::DEAD)) return;

		// If the unit is a foe, set as target.
		if (IMode->player_Team_Number != DesiredTargetUnit->unit_Team_Number) {

			if (TargetUnits[0] != DesiredTargetUnit) {

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

		//_target_mutex.Lock();
		if (_target_mutex.TryLock()) {
			TargetUnits[0] = nullptr;
			_target_mutex.Unlock();
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
				TEXT("Failed Lock!!"));
		}

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
	else {
		StopMovement();
	}
}

void ABaseMeleeUnit::StopMovement() {
	GetCharacterMovement()->StopMovementImmediately();
	Astar->ClearRoute();
}

void ABaseMeleeUnit::BasicAttack() {
	FHitResult RayCastingResult;
	FVector TargetLocation;

	_target_mutex.Lock();
	if (IsValid(TargetUnits[0])) {
		if (TargetUnits[0]->UnitStat->DeadOrAlive == DOA::ALIVE) {
			TargetLocation = TargetUnits[0]->GetActorLocation();
			_target_mutex.Unlock();
		}
		else {
			TargetUnits[0] = nullptr;
			TurnOffBehavior(UNIT_BEHAVIOR::BASICATTACK_ORDER);
			_target_mutex.Unlock();
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

void ABaseMeleeUnit::DestroyCaller() {
	Weapon->Destroy();
	Destroy();
}

void ABaseMeleeUnit::SkillActivator() {
	// Check target validation.
	if (!IsValid(TargetUnits[0])) return;

	float distance = FMath::Abs(FVector::Distance(TargetUnits[0]->GetActorLocation(), GetActorLocation()));

	// Check target is in skill range.
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
	else {
		FollowTarget();
	}
}

void ABaseMeleeUnit::SkillAttackCheck() {
	_attack_mutex.Lock();
	if (IsValid(TargetUnits[0]) 
		&& TargetUnits[0]->UnitStat->DeadOrAlive == DOA::ALIVE) {
		float amount = UAttackCaculator::SkillDamage(this, TargetUnits[0]);
		FDamageEvent DamageEvent;
		TargetUnits[0]->TakeDamage(amount, DamageEvent, GetController(), this);
	}
	_attack_mutex.Unlock();
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
	// Can't be target is myself.
	if (TargetUnits[0] == this) return;
	
	TurnOnBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE_ORDER);
	

	// Move to location.
	FVector TargetLocation;
	_target_mutex.Lock();
	if (TargetUnits[0]->UnitStat->DeadOrAlive == DOA::ALIVE) {
		TargetLocation = TargetUnits[0]->GetActorLocation();
	}
	_target_mutex.Unlock();

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





void ABaseMeleeUnit::Debug_FogVision() {
	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	if (IMode == nullptr || (IMode->player_Team_Number != unit_Team_Number)) return;

	auto FogMgr = FogRegister->Manager;

	uint32 halfTextureSize = FogMgr->TextureSize / 2;
	int signedSize = (int)FogMgr->TextureSize;
	int sightTexels = SightRange;
	int size = (int)FogMgr->TextureSize;
	float dividend = FogMgr->TexelUnit;
	float tSize = FogMgr->TexelUnit;

	TSet<FVector2D> currentlyInSight;
	TSet<FVector2D> texelsToBlur;

	FVector position = GetActorLocation();


	// Change world location to texture location.
	int posX = (int)(position.X / dividend) + halfTextureSize;
	int posY = (int)(position.Y / dividend) + halfTextureSize;

	FVector2D textureSpacePos = FVector2D(posX, posY);

	FCollisionQueryParams LayTraceParams(FName(TEXT("LayTraceParams")), false, this);



	//Unveil the positions our actors are currently looking at
	for (int y = posY - sightTexels; y <= posY + sightTexels; y++) {
		for (int x = posX - sightTexels; x <= posX + sightTexels; x++) {

			FVector currentWorldSpacePos = FVector(
				((x - (int)halfTextureSize)) * dividend,
				((y - (int)halfTextureSize)) * dividend,
				position.Z);

			//Kernel for radial sight
			if (x > 0 && x < size && y > 0 && y < size) {
				FVector2D currentTextureSpacePos = FVector2D(x, y);
				int length = (int)(textureSpacePos - currentTextureSpacePos).Size();


				if (length <= sightTexels) {
				

					
					FHitResult Result;
					AUnit* TUnit = nullptr;
					if (GetWorld()->LineTraceSingleByChannel(Result, position, currentWorldSpacePos,
						ECollisionChannel::ECC_WorldStatic, LayTraceParams)) {
						auto HitActor = Result.GetActor();
						
						if (HitActor->IsA(AUnit::StaticClass())) {
							TUnit = Cast<AUnit>(HitActor);
							DrawDebugBox(GetWorld(), currentWorldSpacePos,
								FVector(tSize, tSize, 2), FColor::Green);

							if(IMode->player_Team_Number != TUnit->unit_Team_Number)
								DrawDebugLine(GetWorld(), position, HitActor->GetActorLocation(),
									FColor::Red, false, -1.f,0,5.f);
						}
						else DrawDebugBox(GetWorld(), currentWorldSpacePos,
							FVector(tSize, tSize, 2), FColor::Red);
					}
					

					else {
						DrawDebugBox(GetWorld(), currentWorldSpacePos,
							FVector(tSize, tSize, 2), FColor::Green);
					}
				}
				// inner texel, but not sight.
				
			}

			else {
				DrawDebugBox(GetWorld(), currentWorldSpacePos,
					FVector(tSize, tSize, 2), FColor::White);
			}
		}
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
	SetActorEnableCollision(false);

	bGoBasicAnimInstance = true;
	DefaultAnimInstance->bDead = true;
	
	_behavior_mutex.Lock();
	Behavior = UNIT_BEHAVIOR::NOTHING;
	_behavior_mutex.Unlock();

	// Set docoes unvisibility.
	HeadUpHPbar->SetVisibility(false);
	SelectionMesh->SetVisibility(false);


	// Clear skill HUD if selected a ally unit.
	auto IController = Cast<AMainController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(IController)) {
		if (IController->IsOnlyOneAllyUnitSelected()) {
			
			if (IController->Units[0] == this) {
				IController->SetSkillPanelVisibility(false);
			}
		}
	}

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ABaseMeleeUnit::DestroyCaller, 5.f, false);
}

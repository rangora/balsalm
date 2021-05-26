// Fill out your copyright notice in the Description page of Project Settings.


#include "AxeSkillAnimInstance.h"
#include "MeleeAnimInstance.h"
#include "Engine/Engine.h"
#include "../BaseMeleeUnit.h"
#include "../Equipment/BaseWeapon.h"
#include "../../../System/Handler/SkillObject.h"



UAxeSkillAnimInstance::UAxeSkillAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SkullCrack(
		TEXT("/Game/Mannequin/Animations/AM_Axe_SkullCrack.AM_Axe_SkullCrack"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_CycloneAxe(
		TEXT("/Game/Mannequin/Animations/AM_Axe_CycloneAxe.AM_Axe_CycloneAxe"));

	SkillCrack_Hit = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SkillCrack_Hit"));
	CycloneAxe_Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CycloneAxe_Effect"));

	if (MON_SkullCrack.Succeeded()) 
		SkullCrackMontage = MON_SkullCrack.Object;
	if (MON_CycloneAxe.Succeeded())
		CycloneAxeMontage = MON_CycloneAxe.Object;
}

void UAxeSkillAnimInstance::PlaySkullCrack() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	aUnit->TurnOffBehavior(UNIT_BEHAVIOR::MOVABLE);
	aUnit->TurnOffBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);
	Montage_Play(SkullCrackMontage, 1.7f);
}

void UAxeSkillAnimInstance::PlayCycloneAxe() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	aUnit->TurnOnBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE);
	aUnit->TurnOffBehavior(UNIT_BEHAVIOR::MOVABLE);
	aUnit->TurnOffBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);
	Montage_Play(CycloneAxeMontage, 1.7f);
}

void UAxeSkillAnimInstance::AnimNotify_SkillEnd() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());

	if (IsValid(aUnit)) {
		aUnit->TargetUnits[0] = nullptr;
		aUnit->bGoBasicAnimInstance = true;
		aUnit->TurnOffBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE);
		aUnit->TurnOnBehavior(UNIT_BEHAVIOR::MOVABLE);
	}
}

void UAxeSkillAnimInstance::AnimNotify_SkullCrackHit() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	if (IsValid(aUnit)) {
		FTransform Trans;
		FVector P_Vector;
		FQuat P_Quat;
	
		aUnit->Weapon->EquipmentMesh->GetSocketWorldLocationAndRotation(
			FName("HitLocationSocket"), P_Vector, P_Quat);
		Trans.SetLocation(P_Vector);
		Trans.SetRotation(P_Quat);

		auto InWorld = aUnit->GetWorld();
		UGameplayStatics::SpawnEmitterAtLocation(InWorld,
			SkillCrack_Hit->Template, Trans, true);

		aUnit->SkillAttackCheck();
	}
}

void UAxeSkillAnimInstance::AnimNotify_CycloneAxeHit() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	if (IsValid(aUnit)) {
		auto InWorld = aUnit->GetWorld();	
		FTransform Trans;		
		FVector P_Vector = aUnit->GetActorForwardVector() * 200.f + aUnit->GetActorLocation();

		Trans.SetLocation(P_Vector);
		
		UGameplayStatics::SpawnEmitterAtLocation(InWorld,
			CycloneAxe_Effect->Template, Trans, true);
		aUnit->SkillRef->AreaSkillJudge(aUnit);
	}
}

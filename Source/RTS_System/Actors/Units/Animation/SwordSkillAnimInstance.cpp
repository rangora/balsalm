// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordSkillAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "../BaseMeleeUnit.h"



USwordSkillAnimInstance::USwordSkillAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_DoubleStrike(
		TEXT("/Game/Mannequin/Animations/AM_Sword_DoubleStrike.AM_Sword_DoubleStrike"));

	DoubleStrike_Hit_Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DoubleStrike_Hit_Effect"));

	if (MON_DoubleStrike.Succeeded())
		DoubleStrikeMontage = MON_DoubleStrike.Object;
}


void USwordSkillAnimInstance::PlayDoubleStrike() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	aUnit->TurnOffBehavior(UNIT_BEHAVIOR::MOVABLE);
	aUnit->TurnOffBehavior(UNIT_BEHAVIOR::SKILL_TARGETING);
	Montage_Play(DoubleStrikeMontage, 1.7f);
}

void USwordSkillAnimInstance::AnimNotify_DoubleStrike_FirstHit() {
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
			DoubleStrike_Hit_Effect->Template, Trans, true);

		aUnit->SkillAttackCheck();
	}
}

void USwordSkillAnimInstance::AnimNotify_DoubleStrike_SecondHit() {
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
			DoubleStrike_Hit_Effect->Template, Trans, true);

		aUnit->SkillAttackCheck();
	}
}

void USwordSkillAnimInstance::AnimNotify_SkillEnd() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());

	if (IsValid(aUnit)) {
		aUnit->TargetUnits[0] = nullptr;
		aUnit->bGoBasicAnimInstance = true;
		aUnit->TurnOffBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE);
		aUnit->TurnOnBehavior(UNIT_BEHAVIOR::MOVABLE);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AxeSkillAnimInstance.h"
#include "MeleeAnimInstance.h"
#include "../BaseMeleeUnit.h"
#include "../Equipment/BaseWeapon.h"

UAxeSkillAnimInstance::UAxeSkillAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_SkullCrack(
		TEXT("/Game/Mannequin/Animations/AM_Axe_SkullCrack.AM_Axe_SkullCrack"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MON_CycloneAxe(
		TEXT("/Game/Mannequin/Animations/AM_Axe_CycloneAxe.AM_Axe_CycloneAxe"));
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> P_SkullCrackHit(
	//	TEXT("/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2"));

	SkillCrack_Hit = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffect"));

	if (MON_SkullCrack.Succeeded()) 
		SkullCrackMontage = MON_SkullCrack.Object;
	if (MON_CycloneAxe.Succeeded())
		CycloneAxeMontage = MON_CycloneAxe.Object;

	SkillCrack_Hit->bAutoActivate = false;
	SkillCrack_Hit->SetCollisionProfileName("NoCollision");
}

void UAxeSkillAnimInstance::PlaySkullCrack() {
	Montage_Play(SkullCrackMontage, 2.0f);
}

void UAxeSkillAnimInstance::PlayCycloneAxe() {
	Montage_Play(CycloneAxeMontage, 1.f);
}

void UAxeSkillAnimInstance::AnimNotify_SkillEnd() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());

	if (IsValid(aUnit)) {
		aUnit->bGoBasicAnimInstance = true;
		aUnit->TurnOffBehavior(UNIT_BEHAVIOR::SKILL_ACTIVE);
		aUnit->TurnOnBehavior(UNIT_BEHAVIOR::MOVABLE);
	}
}

void UAxeSkillAnimInstance::AnimNotify_SkullCrackHit() {
	auto aUnit = Cast<ABaseMeleeUnit>(GetOwningActor());
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("AnimNotify_SkillCheck"));
	if (IsValid(aUnit)) {
		/*FVector HitLocation;
		
		aUnit->GetHitLocation(HitLocation);
		if (HitLocation != FVector::ZeroVector) {*/
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("active"));
			
		aUnit->Weapon->ActiveHitParticle(SkillCrack_Hit);
		//auto WeaponSkeletal = aUnit->Weapon->EquipmentSkeletal;

		//SkillCrack_Hit->AttachToComponent(WeaponSkeletal, );
		/*SkillCrack_Hit->AttachTo(WeaponSkeletal, "HitLocationSocket");
		SkillCrack_Hit->Activate();*/


	}
	//}
}

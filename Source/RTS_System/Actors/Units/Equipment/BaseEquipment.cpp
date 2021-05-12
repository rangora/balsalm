// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEquipment.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystemComponent.h"



// Sets default values
ABaseEquipment::ABaseEquipment() : ItemType(ITEMTYPE::EMPTY) {
	PrimaryActorTick.bCanEverTick = false;

	//EquipmentSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletal"));
	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	HitParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponHitEffect"));

	//EquipmentSkeletal->SetCollisionProfileName(TEXT("NoCollision"));
	EquipmentMesh->SetCollisionProfileName(TEXT("NoCollision"));
	EquipmentMesh->SetupAttachment(RootComponent);

	HitParticle->bAutoActivate = false;
	HitParticle->SetCollisionProfileName("NoCollision");
	HitParticle->SetupAttachment(EquipmentMesh, FName("HitLocationSocket"));
}

void ABaseEquipment::BeginPlay() {
	Super::BeginPlay();
}

void ABaseEquipment::ActiveHitParticle(UParticleSystemComponent* pParticle) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("ActiveHitParticle"));

	FAttachmentTransformRules SocketRule(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);

	//pParticle->SetupAttachment(EquipmentMesh);
	pParticle->SetupAttachment(EquipmentMesh, FName("HitLocationSocket"));
	if(pParticle->IsAttachedTo(EquipmentMesh)) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("IsAttachedTo"));
		FString name;
		pParticle->GetName(name);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(
			TEXT("name:%s"), *name));

		HitParticle->Activate(true);
		pParticle->Activate(true);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("No IsAttachedTo"));
	}
	//pParticle->AttachToComponent(EquipmentSkeletal, SocketRule, "HitLocationSocket");
	//pParticle->AttachTo(EquipmentSkeletal, "HitLocationSocket");
	
}


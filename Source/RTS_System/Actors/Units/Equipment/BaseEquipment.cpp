// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEquipment.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystemComponent.h"



// Sets default values
ABaseEquipment::ABaseEquipment() : ItemType(ITEMTYPE::EMPTY) {
	PrimaryActorTick.bCanEverTick = false;

	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	HitParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WeaponHitEffect"));

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
	if(IsValid(pParticle)) {
		HitParticle->SetTemplate(pParticle->Template);
		HitParticle->SetRelativeScale3D(pParticle->GetRelativeScale3D());
		HitParticle->Activate(true);
	}
}


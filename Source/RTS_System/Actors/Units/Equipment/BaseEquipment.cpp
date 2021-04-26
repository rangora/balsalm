// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEquipment.h"

// Sets default values
ABaseEquipment::ABaseEquipment() : ItemType(ITEMTYPE::EMPTY) {
	PrimaryActorTick.bCanEverTick = false;

	EquipmentSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletal"));
	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	EquipmentSkeletal->SetCollisionProfileName(TEXT("NoCollision"));
	EquipmentMesh->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = EquipmentSkeletal;
}

void ABaseEquipment::BeginPlay() {
	Super::BeginPlay();
}


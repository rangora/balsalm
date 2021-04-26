// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

ABaseWeapon& ABaseWeapon::operator=(const ABaseWeapon& ref) {
	this->EquipmentSkeletal = ref.EquipmentSkeletal;
	this->EquipmentMesh = ref.EquipmentMesh;
	this->ItemType = ref.ItemType;
	
	WeaponDamage = ref.WeaponDamage;
	WeaponRange = ref.WeaponRange;
	WeaponSpeed = ref.WeaponSpeed;
	WeaponType = ref.WeaponType;
	
	UE_LOG(LogTemp, Log, TEXT("ABaseWeapon operator= called.."));

	return *this;
}
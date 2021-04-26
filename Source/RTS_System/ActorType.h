// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

UENUM(BlueprintType)
enum class DOA :uint8 {
	DEAD UMETA(DisplayName = "DEAD"),
	ALIVE UMETA(DisplayName = "ALIVE"),
};

UENUM(BlueprintType)
enum class ITEMTYPE : uint8 {
	EMPTY UMETA(DisplayName = "EMPTY"),
	WEAPON UMETA(DisplayName = "WEAPON"),
	ARMOR UMETA(DisplayName = "ARMOR"),
};

UENUM(BlueprintType)
enum class WEAPONTYPE : uint8 {
	SWORD UMETA(DisplayName = "SWORD"),
	AXE UMETA(DisplayName = "AXE"),
	BOW UMETA(DisplayName = "BOW"),
	GUN UMETA(DisplayName = "GUN"),
};

UENUM(BlueprintType)
enum class ARMORTYPE : uint8 {
	HELEM UMETA(DisplayName = "HELEM"),
	TORSO UMETA(DisplayName = "TORSO"),
	LEGGING UMETA(DisplayName = "LEGGING"),
};

UENUM(BlueprintType)
enum class ATTACKTYPE : uint8 {
	CUT UMETA(DisplayName = "CUT"),
	CRUSH UMETA(DisplayName = "CRUSH"),
	PIERCE UMETA(DisplayName = "PIERCE"),
};

UENUM(BlueprintType)
enum class SKILLTYPE : uint8 {
	WEAPON_MELEE UMETA(DisplayName = "MeleeWeapon"),
	WEAPON_RANGE UMETA(DisplayName = "RangeWeapon"),
	UTILITY UMETA(DisplayName = "Utility"),
};

UENUM(BlueprintType)
enum class SKILLSHAPE : uint8 {
	WEAPON_MELEE UMETA(DisplayName = "Square"),
	WEAPON_RANGE UMETA(DisplayName = "Circle"),
	UTILITY UMETA(DisplayName = "Sector"),
};

//
///**
// * 
// */
//class RTS_SYSTEM_API ActorType
//{
//public:
//	ActorType();
//	~ActorType();
//};

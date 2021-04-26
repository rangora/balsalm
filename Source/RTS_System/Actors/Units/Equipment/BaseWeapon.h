// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEquipment.h"
#include "BaseWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API ABaseWeapon : public ABaseEquipment {
	GENERATED_BODY()

public:



	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = ItemStat)
		float WeaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = ItemStat)
		float WeaponRange;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = ItemStat)
		float WeaponSpeed;
	UPROPERTY(EditAnywhere, Category = ItemStat)
		WEAPONTYPE WeaponType;
	UPROPERTY(EditAnywhere, Category = ItemStat)
		ATTACKTYPE AttackType;
};

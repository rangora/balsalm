// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"


UStatComponent::UStatComponent()
	: DeadOrAlive(DOA::ALIVE),
	level(1),
	maxHP(100),
	currentHP(100),
	currentDamage(10),
	attackRange(15),
	attackSpeed(2),
	Armor(0) {
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatComponent::TakeDamage(float totalDamage) {
	_mutex.Lock();
	UE_LOG(LogTemp, Log, TEXT("Hit!"));
	if (DeadOrAlive == DOA::ALIVE) {
		float updateHP = FMath::Clamp<float>(currentHP - totalDamage, 0.f, maxHP);
		currentHP = updateHP;
	}
	UE_LOG(LogTemp, Log, TEXT("HP:%f"),currentHP);

	if (currentHP < KINDA_SMALL_NUMBER) {
		currentHP = 0.f;
		DeadOrAlive = DOA::DEAD;
		UE_LOG(LogTemp, Log, TEXT("Dead!"));
	}
	if (Dele_HPWidgetUpdate.IsBound())
		Dele_HPWidgetUpdate.Execute();
	_mutex.Unlock();
}
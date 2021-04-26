// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAnimInstance.h"

UUnitAnimInstance::UUnitAnimInstance()
	: MovementSpeed(0.f) {}

void UUnitAnimInstance::NativeUpdateAnimation(float delta) {
	Super::NativeUpdateAnimation(delta);

	auto IUnit = TryGetPawnOwner();
	if (IsValid(IUnit)) {
		MovementSpeed = IUnit->GetVelocity().Size();
	}
}

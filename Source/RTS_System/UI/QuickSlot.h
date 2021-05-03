// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonSlot.h"
#include "QuickSlot.generated.h"

/**
 * 
 */
UCLASS()
class RTS_SYSTEM_API UQuickSlot : public UCommonSlot {
	GENERATED_BODY()
public:
	void UpdateLinkSlot();

	UPROPERTY()
		UWidget* OriginSlot = nullptr;

	UPROPERTY(EditAnywhere)
		int32 RefIndex;
};

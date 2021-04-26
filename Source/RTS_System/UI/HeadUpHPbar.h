// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HeadUpHPbar.generated.h"

/**
 * 
 */

class UStatComponent;

UCLASS()
class RTS_SYSTEM_API UHeadUpHPbar : public UUserWidget {
	GENERATED_BODY()
		
public:
	virtual void NativeConstruct() override;
	void BindWithOwner(UStatComponent* IStat);

	UFUNCTION()
		void UpdateHPWidget();


	UPROPERTY()
		UProgressBar* HPProgressBar;
	
	UStatComponent* OwnerStat;
};

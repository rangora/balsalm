// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScreenUI.generated.h"

/**
 * 
 */

class UGridPanel;

UCLASS()
class RTS_SYSTEM_API UScreenUI : public UUserWidget {
	GENERATED_BODY()
public:
	UScreenUI(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float delta) override;
	void SetSkillPanelVisibility(bool trigger);

	UPROPERTY()
		UGridPanel* SkillPanel;
};

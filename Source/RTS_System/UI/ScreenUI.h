// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FogManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ScreenUI.generated.h"

/**
 * 
 */

class UGridPanel;
class AUnit;


UCLASS()
class RTS_SYSTEM_API UScreenUI : public UUserWidget {
	GENERATED_BODY()
public:
	UScreenUI(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float delta) override;
	
	void SetSkillPanelVisibility(bool trigger);
	
	// Functions which manipulate skill slots.
	void UpdateSkillSlots(const AUnit* pUnit);
	void CleanAllSlots();

	UPROPERTY()
		UGridPanel* SkillPanel;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (BindWidget))
		UImage* WorldMap;

	UPROPERTY()
		AFogManager* FogMgr = nullptr;
};

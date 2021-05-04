// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "../Actors/Units/Unit.h"
#include "GameFramework/PlayerController.h"
#include "MainController.generated.h"



class UUserWidget;



UCLASS()
class RTS_SYSTEM_API AMainController : public APlayerController {
	GENERATED_BODY()
	


public:
	AMainController();
	virtual void SetupInputComponent() override;

	void SetSkillPanelVisibility(bool bVisible);
	void OpenOrCloseSkillPanel();

	UFUNCTION()
		void ActiveQuickSlot(int32 idx);

	// Called in HUD.
	UFUNCTION(BlueprintCallable)
		void SetUnits(TArray<AActor*> pUnits, bool bClicked);

	// Only can store allies units.
	UPROPERTY(BlueprintReadWrite)
		TArray<AUnit*> Units;

	// User interface widget.
	UPROPERTY()
		UUserWidget* SkillControlUIWidget;


private:
	// Input delegate for quick slots.
	DECLARE_DELEGATE_OneParam(FQuickSlotAction, int32);

	bool IsOnlyOneAllyUnitSelected();

	// User interface class.
	UPROPERTY()
		class TSubclassOf<UUserWidget> SkillControlUIClass;
};

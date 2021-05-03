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

	void SetSkillPanelVisibility();
	void OpenOrCloseSkillPanel();

	UFUNCTION()
		void ActiveQuickSlot(int32 idx);

	UFUNCTION(BlueprintCallable)
		void SetUnits(TArray<AActor*> pUnits, bool bClicked);

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

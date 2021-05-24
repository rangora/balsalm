// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "../System/MainController.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */

class UUserWidget;
class AUnit;


UENUM(BlueprintType)
enum class LeftButtonAction :uint8 {
	NONE	  UMETA(DisplayName = "NONE"),
	DRAG	  UMETA(DisplayName = "DRAG"),
	TARGETING UMETA(DisplayName = "TARGETING"),
	SKILLAREA UMETA(DisplayName = "SKILLAREA")
};


UCLASS()
class RTS_SYSTEM_API AMainHUD : public AHUD {
	GENERATED_BODY()
public:
	AMainHUD();
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		bool IsClicked(const FVector2D& lh, const FVector2D& rh);
		
	UFUNCTION(BlueprintCallable)
		void MouseLeftButtonActionSwitcher();

	UFUNCTION(BlueprintCallable)
		void SetMouseLeftButtonAction(LeftButtonAction Action);

	void SetDragable(bool bDrag);

	UPROPERTY()
		UUserWidget* ScreenUIWidget;

private:
	UPROPERTY()
		TSubclassOf<UUserWidget> ScreenUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		LeftButtonAction CurrentAction;

	FCriticalSection _mutex;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool bDragable = true;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool bTargeting = false;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool bSkillAreaDirection = false;

};

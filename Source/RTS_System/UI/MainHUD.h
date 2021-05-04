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


UCLASS()
class RTS_SYSTEM_API AMainHUD : public AHUD {
	GENERATED_BODY()
public:
	AMainHUD();
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		bool IsClicked(const FVector2D& lh, const FVector2D& rh);
		


	UPROPERTY()
		UUserWidget* ScreenUIWidget;
	UPROPERTY(BlueprintReadWrite)
		bool bDragable = true;

private:
	UPROPERTY()
		TSubclassOf<UUserWidget> ScreenUIClass;
};

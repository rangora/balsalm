// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Blueprint/UserWidget.h"
#include "../System/MainController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CommonSlot.generated.h"

/**
 * 
 */

class UImage;
class UTexture2D;

const static FString DefaultTexturePath = "/Game/Resources/UI/Box2.Box2";

UCLASS()
class RTS_SYSTEM_API UCommonSlot : public UUserWidget {
	GENERATED_BODY()
public:
	UFUNCTION()
		virtual void SetThumbnailImage();
	virtual void WidgetLinkOperation();

	void SetDefaultThumbnailImage();

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		UImage* ThumbnailImage;
	
	UPROPERTY()
		UTexture2D* CurrentTexture;

	// Init in NativeConstruct().
	TWeakObjectPtr<UUserWidget> WidgetLink;

	bool bAssigned = false;
};

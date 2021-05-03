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
const static FString VisualWidgetPath = "/Game/UI/UI_Slot.UI_Slot_C";


UCLASS()
class RTS_SYSTEM_API UCommonSlot : public UUserWidget {
	GENERATED_BODY()
public:
	UFUNCTION()
		virtual void SetThumbnailImage();
	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	
	virtual void WidgetLinkOperation();
	virtual void OverWrite(const UUserWidget* Origin);
	virtual void DropAction(const UUserWidget* From);

	void SetDefaultThumbnailImage();


	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		UImage* ThumbnailImage;
	
	UPROPERTY()
		UTexture2D* CurrentTexture;

	// Init in NativeConstruct().
	UPROPERTY()
		TWeakObjectPtr<UUserWidget> WidgetLink;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bCopiable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bAssigned = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDragable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bOverWrite = false;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/UserWidget.h"
#include "DragDropWidget.generated.h"

/**
 * 
 */

UCLASS()
class RTS_SYSTEM_API UDragDropWidget : public UDragDropOperation {
	GENERATED_BODY()
public:
	UUserWidget* From = nullptr;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "ScreenUI.h"
#include "../ActorType.h"
#include "../System/MainController.h"
#include "../System/MainGameMode.h"
#include "../Actors/Units/Unit.h"
#include "UMG/Public/Blueprint/WidgetTree.h"
#include "UMG/Public/Components/GridPanel.h"



AMainHUD::AMainHUD() {
	static ConstructorHelpers::FClassFinder<UUserWidget> ScreenUI_C(
		TEXT("/Game/UI/BP_ScreenUI.BP_ScreenUI_C"));
	if (ScreenUI_C.Succeeded())
		ScreenUIClass = ScreenUI_C.Class;
	CurrentAction = LeftButtonAction::DRAG;
}

void AMainHUD::DrawHUD() {
	Super::DrawHUD();
}

void AMainHUD::BeginPlay() {
	Super::BeginPlay();
	
	

	if (ScreenUIClass) {
		ScreenUIWidget = CreateWidget<UUserWidget>(GetWorld(), ScreenUIClass);

		if (ScreenUIWidget)
			ScreenUIWidget->AddToViewport();
	}
}

bool AMainHUD::IsClicked(const FVector2D& lh, const FVector2D& rh) {
	int32_t threshold = 40;
	
	int32_t width = FMath::Abs(rh.X - lh.X);
	int32_t height = FMath::Abs(rh.Y - lh.Y);

	int32_t area = width * height;

	if (area > 40) {
		return false;
	}

	return true;
}

void AMainHUD::SetMouseLeftButtonAction(LeftButtonAction Action) {
	CurrentAction = Action;
}

void AMainHUD::SetDragable(bool bDrag) {
	bDragable = bDrag;
}

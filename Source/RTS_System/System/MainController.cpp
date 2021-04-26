// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "../Actors/Units/Component/DecoComponent.h"
#include "../UI/ScreenUI.h"
#include "../UI/SkillControlUI.h"
#include "../UI/MainHUD.h"
#include "CameraPawn.h"
#include "MainGameMode.h"

AMainController::AMainController() {
	static ConstructorHelpers::FClassFinder<UUserWidget> SkillControlUI_C(
		TEXT("/Game/UI/BP_SkillControlUI.BP_SkillControlUI_C"));
	if (SkillControlUI_C.Succeeded()) {
		SkillControlUIClass = SkillControlUI_C.Class;
		SkillControlUIWidget = CreateWidget<UUserWidget>(GetWorld(), SkillControlUIClass);
	}
}

void AMainController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("OpenSkilUI"), EInputEvent::IE_Pressed, this, &AMainController::OpenOrCloseSkillPanel);
}

void AMainController::SetSkillPanelVisibility() {
	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto IHUD = this->GetHUD<AMainHUD>();
	auto pScreenWidget = Cast<UScreenUI>(IHUD->ScreenUIWidget);
	
	if (Units.Num() == 1 && 
		Units[0]->unit_Team_Number == IMode->player_Team_Number) {
		pScreenWidget->SetSkillPanelVisibility(true);
	}
	else pScreenWidget->SetSkillPanelVisibility(false);
}

void AMainController::OpenOrCloseSkillPanel() {
	auto SkillUI = Cast<USkillControlUI>(SkillControlUIWidget);
	SkillUI->OpenOrClose();
}

void AMainController::SetUnits(TArray<AActor*> pUnits, bool bClicked=false) {
	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AUnit* Foe_Unit=nullptr;
	int32_t idx = 0;

	// Clear the data.
	for (int i = 0; i < Units.Num(); i++) {
		Units[i]->Deco->SelectionMeshRef->SetVisibility(false);
	}
	Units.Empty();

	if (bClicked && pUnits.Num()) idx = pUnits.Num() - 1;

	for (int i = idx; i < pUnits.Num(); i++) {
		auto aUnit = Cast<AUnit>(pUnits[i]);

		if (IsValid(aUnit)) {
			// Allies 
			if (IMode->player_Team_Number == aUnit->unit_Team_Number) {
				Units.Add(aUnit);
				aUnit->Deco->SelectionMeshRef->SetVisibility(true);
			}
			// Foes
			else 
				Foe_Unit = aUnit;
		}
	}

	// Only select a foe.
	if (Units.Num() == 0 && Foe_Unit!=nullptr) {
		Units.Add(Foe_Unit);
		Foe_Unit->Deco->SelectionDynamic->SetVectorParameterValue(TEXT("SelectionColor"), FLinearColor(100.f, 5.f, 0.f));
		Foe_Unit->Deco->SelectionMeshRef->SetVisibility(true);
	}

	// Check wheater draw ScreenUIWidget or not.
	SetSkillPanelVisibility();
}


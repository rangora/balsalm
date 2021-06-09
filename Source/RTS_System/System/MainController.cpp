// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "CameraPawn.h"
#include "MainGameMode.h"
#include "../Actors/Units/Component/DecoComponent.h"
#include "../Actors/Units/Component/StatComponent.h"
#include "../Actors/Units/BaseMeleeUnit.h"
#include "../UI/ScreenUI.h"
#include "../UI/SkillControlUI.h"
#include "../UI/MainHUD.h"
#include "../UI/QuickSlot.h"
#include "../System/Handler/SkillObject.h"
#include "../System/Handler/CoolDownHandler.h"
#include "../System/Handler/AllSkill.h"
#include "../ActorType.h"
#include "../DataTable/ABaseSkillTable.h"
#include "UMG/Public/Components/GridPanel.h"



/* In-build functions.*/

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
	InputComponent->BindAction<FQuickSlotAction>(TEXT("Quick01"), EInputEvent::IE_Pressed, this, &AMainController::ActiveQuickSlot, 1);
	InputComponent->BindAction<FQuickSlotAction>(TEXT("Quick02"), EInputEvent::IE_Pressed, this, &AMainController::ActiveQuickSlot, 2);
	InputComponent->BindAction<FQuickSlotAction>(TEXT("Quick03"), EInputEvent::IE_Pressed, this, &AMainController::ActiveQuickSlot, 3);
	InputComponent->BindAction<FQuickSlotAction>(TEXT("Quick04"), EInputEvent::IE_Pressed, this, &AMainController::ActiveQuickSlot, 4);
}

/**********/




/* User defined functions. */

void AMainController::SetSkillPanelVisibility(bool bVisible) {
	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	auto IHUD = this->GetHUD<AMainHUD>();
	auto pScreenWidget = Cast<UScreenUI>(IHUD->ScreenUIWidget);
	
	if (bVisible) {
		pScreenWidget->SetSkillPanelVisibility(true);
		pScreenWidget->UpdateSkillSlots(Units[0]);
	}
	else {
		pScreenWidget->SetSkillPanelVisibility(false);
		pScreenWidget->CleanAllSlots();
	}
}

void AMainController::OpenOrCloseSkillPanel() {
	auto SkillUI = Cast<USkillControlUI>(SkillControlUIWidget);
	auto MainHUD = Cast<AMainHUD>(GetHUD());

	if (IsOnlyOneAllyUnitSelected()) {
		auto ControllableUnit = Cast<ABaseMeleeUnit>(Units[0]);

		if (IsValid(ControllableUnit)) {
			// UI open action.
			if (SkillUI->OpenOrClose()) {
				SkillUI->UnitSkillConnector(ControllableUnit, true);				
				MainHUD->SetMouseLeftButtonAction(LeftButtonAction::NONE);
			}
			// UI close action.
			else {
				SkillUI->UnitSkillConnector(ControllableUnit, false);
				SkillUI->Clear();
				MainHUD->SetMouseLeftButtonAction(LeftButtonAction::DRAG);

				auto pScreenWidget = Cast<UScreenUI>(MainHUD->ScreenUIWidget);
				pScreenWidget->UpdateSkillSlots(Units[0]);
			}
		}
	}
}

void AMainController::ActiveQuickSlot(int32 idx) {
	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (IsOnlyOneAllyUnitSelected()) {

		auto MainHUD = Cast<AMainHUD>(GetHUD());
		auto pScreenWidget = Cast<UScreenUI>(MainHUD->ScreenUIWidget);
		auto UnitSkillArray = pScreenWidget->SkillPanel->GetAllChildren();

		// Quickslot start zero index.
		auto InputedQuickSlot = Cast<UQuickSlot>(UnitSkillArray[idx - 1]);

		if (IsValid(InputedQuickSlot)) {
			auto UnitSkillObject = Cast<UQuickSlot>(UnitSkillArray[idx - 1])->SkillObject;

			// Is it empty quickSlot ?
			if (!IsValid(UnitSkillObject)) return;

			// Is it on cooldown ?
			if(UnitSkillObject->bCoolDown()) return;


			if (IsValid(UnitSkillObject)) {
				Units[0]->SkillRef = UnitSkillObject;				
				UnitSkillObject->SkillActivator(Units[0]);
			}
		}
	}
}

void AMainController::SetUnits(TArray<AActor*> pUnits, bool bClicked=false) {
	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AUnit* Foe_Unit=nullptr;
	int32_t idx = 0;
	bool bShowSkillPanel = false;

	// Clear the data.
	for (int i = 0; i < Units.Num(); i++) {
		Units[i]->Deco->SelectionMeshRef->SetVisibility(false);
	}
	Units.Empty();

	if (bClicked && pUnits.Num()) idx = pUnits.Num() - 1;

	for (int i = idx; i < pUnits.Num(); i++) {
		auto aUnit = Cast<AUnit>(pUnits[i]);

		if (IsValid(aUnit)) {
			// Check daead or alive.
			if (aUnit->UnitStat->DeadOrAlive == DOA::DEAD) continue;

			// Allies 
			if (IMode->player_Team_Number == aUnit->unit_Team_Number) {
				Units.Add(aUnit);
				aUnit->Deco->SelectionMeshRef->SetVisibility(true);
			}
			// Foes
			else Foe_Unit = aUnit;
		}
	}

	// Only select a foe.
	if (Units.Num() == 0 && Foe_Unit!=nullptr) {
		Units.Add(Foe_Unit);
		Foe_Unit->Deco->SelectionDynamic->SetVectorParameterValue(TEXT("SelectionColor"), FLinearColor(100.f, 5.f, 0.f));
		Foe_Unit->Deco->SelectionMeshRef->SetVisibility(true);
	}
	// Jobs when only one ally unit selected..
	else if (Units.Num() == 1) 
		bShowSkillPanel = true;
	
	// Check wheater draw ScreenUIWidget or not.
	SetSkillPanelVisibility(bShowSkillPanel);
}

bool AMainController::IsOnlyOneAllyUnitSelected() {
	auto IMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	if (Units.Num() == 1
		&& Units[0]->unit_Team_Number == IMode->player_Team_Number) {
		return true;
	}
	return false;
}





/**************/


/* Private functions. */


/**************/


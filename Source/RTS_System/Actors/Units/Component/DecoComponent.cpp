// Fill out your copyright notice in the Description page of Project Settings.


#include "DecoComponent.h"
#include "Engine.h"
#include "StatComponent.h"
#include "../Unit.h"
#include "../../../UI/HeadUpHPbar.h"


UDecoComponent::UDecoComponent() {
	PrimaryComponentTick.bCanEverTick = false;

	/*static ConstructorHelpers::FClassFinder< UHeadUpHPbar> UI_TEMP(
		TEXT("/Game/UI/UI_HeadHP.UI_HeadHP_C"));
	auto loc = UI_TEMP.Class;*/
	
}
	



// Called when the game starts
void UDecoComponent::BeginPlay() {
	Super::BeginPlay();
	
	auto IOwner = Cast<AUnit>(GetOwner());
	static FString BP_HeadUpHPbarPath = "/Game/UI/UI_HeadHP.UI_HeadHP_C";

	// Init Reference.
	SelectionMeshRef = IOwner->SelectionMesh;
	HeadUpHPbarRef = IOwner->HeadUpHPbar;
	AttackRadiusRef = IOwner->AttackRadius;
	
	auto BP_HeadUpHPbar = StaticLoadClass(UUserWidget::StaticClass(), NULL, *BP_HeadUpHPbarPath);
	
	SelectionMeshRef->SetCollisionProfileName("NoCollision");
	SelectionMeshRef->SetVisibility(false);

	SelectionDynamic = UMaterialInstanceDynamic::Create(SelectionMeshRef->GetMaterial(0), NULL);
	SelectionMeshRef->SetMaterial(0, SelectionDynamic);

	HeadUpHPbarRef->SetWidgetClass(BP_HeadUpHPbar);
	HeadUpHPbarRef->SetRelativeLocation(FVector(0.f,0.f,200.f));
	HeadUpHPbarRef->SetWidgetSpace(EWidgetSpace::Screen);
	HeadUpHPbarRef->SetDrawSize(FVector2D(120.f, 50.f));
	auto TmpHPbarWidget = Cast<UHeadUpHPbar>(HeadUpHPbarRef->GetUserWidgetObject());
	
	if (IsValid(TmpHPbarWidget)) {
		TmpHPbarWidget->BindWithOwner(IOwner->UnitStat);
		IOwner->UnitStat->Dele_HPWidgetUpdate.Execute();
	}
	UpdateAttackRadius();
}

void UDecoComponent::UpdateAttackRadius() {
	auto IOwner = Cast<AUnit>(GetOwner());
	float attackRadius = IOwner->UnitStat->attackRange;

	AttackRadiusRef->SetSphereRadius(attackRadius);
}


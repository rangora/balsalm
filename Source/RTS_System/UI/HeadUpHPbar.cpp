// Fill out your copyright notice in the Description page of Project Settings.


#include "HeadUpHPbar.h"
#include"../Actors/Units/Unit.h"
#include"../Actors/Units/Component/StatComponent.h"


void UHeadUpHPbar::NativeConstruct() {
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	UpdateHPWidget();
}

void UHeadUpHPbar::BindWithOwner(UStatComponent* IStat) {
	OwnerStat = IStat;
	IStat->Dele_HPWidgetUpdate.BindUFunction(this, FName("UpdateHPWidget"));
}

void UHeadUpHPbar::UpdateHPWidget() {
	if(IsValid(OwnerStat))
		HPProgressBar->SetPercent(OwnerStat->currentHP / OwnerStat->maxHP);
}



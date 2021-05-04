// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonSlot.h"
#include "../ActorType.h"
#include "WeaponListSlot.generated.h"

/**
 * 
 */


UCLASS()
class RTS_SYSTEM_API UWeaponListSlot : public UCommonSlot {
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	

	virtual void WidgetLinkOperation() override;
	virtual void DropAction(const UUserWidget* From) override;

	void bBorderImageVisible(bool visibility);
	void SetThisWeapon(WEAPONTYPE pWeaponType);

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		UImage* BorderImage;

	WEAPONTYPE WeaponType;
	static UWeaponListSlot* SelectedSlot;
	

private:
	enum WEAPON_INDEX {
		AXE,
		GUN
	};
};

UWeaponListSlot* UWeaponListSlot::SelectedSlot = nullptr;

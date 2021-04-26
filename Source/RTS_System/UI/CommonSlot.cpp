// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonSlot.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"


void UCommonSlot::SetThumbnailImage() {
	if (!IsValid(CurrentTexture)) return;
	ThumbnailImage->SetBrushFromTexture(CurrentTexture);
}

void UCommonSlot::WidgetLinkOperation() {}

void UCommonSlot::SetDefaultThumbnailImage() {
	// TEST
	auto Tex = LoadObject<UTexture2D>(NULL, *DefaultTexturePath, NULL, LOAD_None, NULL);
	ThumbnailImage->SetBrushFromTexture(Tex);
}

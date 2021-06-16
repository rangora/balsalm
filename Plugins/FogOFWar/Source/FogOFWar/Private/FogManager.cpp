// Fill out your copyright notice in the Description page of Project Settings.


#include "FogManager.h"
#include "Engine/Engine.h"
#include "FogOfWarWorker.h"
#include "FogRegister.h"

AFogManager::AFogManager() {
	PrimaryActorTick.bCanEverTick = true;
	
	textureRegions = new FUpdateTextureRegion2D(0, 0, 0, 0, TextureSize, TextureSize);

	//15 Gaussian samples. Sigma is 2.0.
	//CONSIDER: Calculate the kernel instead, more flexibility...
	blurKernel.Init(0.0f, blurKernelSize);
	blurKernel[0] = 0.000489f;
	blurKernel[1] = 0.002403f;
	blurKernel[2] = 0.009246f;
	blurKernel[3] = 0.02784f;
	blurKernel[4] = 0.065602f;
	blurKernel[5] = 0.120999f;
	blurKernel[6] = 0.174697f;
	blurKernel[7] = 0.197448f;
	blurKernel[8] = 0.174697f;
	blurKernel[9] = 0.120999f;
	blurKernel[10] = 0.065602f;
	blurKernel[11] = 0.02784f;
	blurKernel[12] = 0.009246f;
	blurKernel[13] = 0.002403f;
	blurKernel[14] = 0.000489f;
}

void AFogManager::BeginPlay() {
	Super::BeginPlay();
	
	bIsDoneBlending = true;
	AFogManager::StartFOWTextureUpdate();
}

AFogManager::~AFogManager() {
	if (FowThread) {
		FowThread->ShutDown();
	}
}

void AFogManager::StartFOWTextureUpdate() {
	if (!FOWTexture) {
		FOWTexture = UTexture2D::CreateTransient(TextureSize, TextureSize);
		LastFOWTexture = UTexture2D::CreateTransient(TextureSize, TextureSize);
		
		int arraySize = TextureSize * TextureSize;
		
		TextureData.Init(FColor(0, 0, 0, 255), arraySize);
		LastFrameTextureData.Init(FColor(0, 0, 0, 255), arraySize);
		HorizontalBlurData.Init(0, arraySize);
		UnfoggedData.Init(false, arraySize);
		FowThread = new AFogOfWarWorker(this);

		FOWArray.Init(false, arraySize);
	}
}

bool AFogManager::GetIsBlurEnabled() {
	return bIsBlurEnabled;
}

void AFogManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	int n = 4;

	// Be sure it have safe texture and blended updated.
	if (FOWTexture && LastFOWTexture && bHasFOWTextureUpdate && bIsDoneBlending) {
		LastFOWTexture->UpdateResource();
		UpdateTextureRegions(LastFOWTexture, (int32)0, (uint32)1, textureRegions, (uint32)(n * TextureSize), (uint32)n, (uint8*)LastFrameTextureData.GetData(), false);
		FOWTexture->UpdateResource();
		UpdateTextureRegions(FOWTexture, (int32)0, (uint32)1, textureRegions, (uint32)(n * TextureSize), (uint32)n, (uint8*)TextureData.GetData(), false);
		bHasFOWTextureUpdate = false;
		bIsDoneBlending = false;
		//Trigger the blueprint update
		OnFowTextureUpdated(FOWTexture, LastFOWTexture);
	}
}

void AFogManager::OnFowTextureUpdated_Implementation(UTexture2D* currentTexture, UTexture2D* lastTexture) {}

void AFogManager::RegisterFowActor(AActor* Actor) {
	FowActors.Add(Actor);
}

void AFogManager::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData) {
	if (Texture && Texture->Resource) {
		struct FUpdateTextureRegionsData {
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_RENDER_COMMAND(FUpdateTextureRegionsData)(
			[RegionData = RegionData, bFreeData = bFreeData](FRHICommandListImmediate& RHICmdList) {
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex) {
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip) {
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}
				if (bFreeData) {
					FMemory::Free(RegionData->Regions);
					FMemory::Free(RegionData->SrcData);
				}
				delete RegionData;
			});
	}
}

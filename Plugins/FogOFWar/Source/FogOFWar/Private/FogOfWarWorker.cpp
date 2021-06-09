// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWarWorker.h"
#include "FogManager.h"
#include "FogRegister.h"
#include "../../../../../Source/RTS_System/Actors/Units/Unit.h"
//#include "../../../../../Source/RTS_System/System/MainGameMode.h"
#include "HAL/RunnableThread.h"
#include "Engine/Engine.h"


AFogOfWarWorker::AFogOfWarWorker() {}

AFogOfWarWorker::AFogOfWarWorker(AFogManager* Param_Manager) {
	Manager = Param_Manager;
	Thread = FRunnableThread::Create(this, TEXT("FogOfWarWorker"), 0U, TPri_BelowNormal);
}

AFogOfWarWorker::~AFogOfWarWorker() {
	delete Thread;
	Thread = NULL;
}

uint32 AFogOfWarWorker::Run() {
	FPlatformProcess::Sleep(0.03f);
	while (StopTaskCounter.GetValue() == 0) {
		//the compiler was complaining about the time variable not being initiallized, so = 0.0f
		float time = 0.0f;
		if (Manager && Manager->GetWorld()) {
			time = Manager->GetWorld()->TimeSeconds;
		}
		if (!Manager->bHasFOWTextureUpdate) {
			UpdateFowTexture();
			if (Manager && Manager->GetWorld()) {
				Manager->fowUpdateTime = Manager->GetWorld()->TimeSince(time);
			}
		}
		FPlatformProcess::Sleep(0.1f);
	}
	return 0;
}

void AFogOfWarWorker::Stop() {
	StopTaskCounter.Increment();
}

void AFogOfWarWorker::ShutDown() {
	Stop();
	Thread->WaitForCompletion();
}

void AFogOfWarWorker::UpdateFowTexture() {
	Manager->LastFrameTextureData = TArray<FColor>(Manager->TextureData);
	uint32 halfTextureSize = Manager->TextureSize / 2; 
	int signedSize = (int)Manager->TextureSize; 
	float dividend = Manager->TexelUnit / Manager->SamplesPerMeter; 

	TSet<FVector2D> currentlyInSight;
	TSet<FVector2D> texelsToBlur;
	


	// Get customer.
	for (auto Itr(Manager->FowActors.CreateIterator()); Itr; Itr++) {
		// if you experience an occasional crash
		if (StopTaskCounter.GetValue() != 0) {
			return;
		}
		//Find actor position
		if (!*Itr) return;
		
		AUnit* IUnit = Cast<AUnit>((*Itr));
		FVector position = (*Itr)->GetActorLocation();

		int sightTexels = IUnit->SightRange * Manager->SamplesPerMeter;

		//3200 1600

		// 현실 좌표를 텍스쳐 좌표로 변환
		int posX = (int)(position.X / dividend) + halfTextureSize; // x/50 + 512
		int posY = (int)(position.Y / dividend) + halfTextureSize;

		FVector2D textureSpacePos = FVector2D(posX, posY);
		int size = (int)Manager->TextureSize;

		FCollisionQueryParams LayTraceParams(FName(TEXT("fow laytrace Params")), false, (*Itr));


		int halfKernelSize = (Manager->blurKernelSize - 1) / 2; // 7
		

		//Store the positions we want to blur
		for (int y = posY - sightTexels - halfKernelSize; y <= posY + sightTexels + halfKernelSize; y++) {
			for (int x = posX - sightTexels - halfKernelSize; x <= posX + sightTexels + halfKernelSize; x++) {
				if (x > 0 && x < size && y > 0 && y < size) {
					texelsToBlur.Add(FIntPoint(x, y));
				}
			}
		}

		//Unveil the positions our actors are currently looking at
		for (int y = posY - sightTexels; y <= posY + sightTexels; y++) {
			for (int x = posX - sightTexels; x <= posX + sightTexels; x++) {
				

				//Kernel for radial sight
				if (x > 0 && x < size && y > 0 && y < size) {
					FVector2D currentTextureSpacePos = FVector2D(x, y);
					int length = (int)(textureSpacePos - currentTextureSpacePos).Size();
					if (length <= sightTexels) {
						FVector currentWorldSpacePos = FVector(
							((x - (int)halfTextureSize)) * dividend,
							((y - (int)halfTextureSize)) * dividend,
							position.Z);

						//CONSIDER: This is NOT the most efficient way to do conditional unfogging. With long view distances and/or a lot of actors affecting the FOW-data
						//it would be preferrable to not trace against all the boundary points and internal texels/positions of the circle, but create and cache "rasterizations" of
						//viewing circles (using Bresenham's midpoint circle algorithm) for the needed 
						// s, shift the circles to the actor's location
						//and just trace against the boundaries.
						//We would then use Manager->GetWorld()->LineTraceSingle() and find the first collision texel. Having found the nearest collision
						//for every ray we would unveil all the points between the collision and origo using Bresenham's Line-drawing algorithm.
						//However, the tracing doesn't seem like it takes much time at all (~0.02ms with four actors tracing circles of 18 texels each),
						//it's the blurring that chews CPU..

						FHitResult Results;
						AUnit* TUnit = nullptr;
						if (Manager->GetWorld()->LineTraceSingleByChannel(Results, position, currentWorldSpacePos,
							ECollisionChannel::ECC_WorldStatic, LayTraceParams)) {

							auto TActor = Results.GetActor();
							if (TActor->IsA(AUnit::StaticClass())) {
								TUnit = Cast<AUnit>(TActor);
								Manager->UnfoggedData[x + y * Manager->TextureSize] = true;
								currentlyInSight.Add(FVector2D(x, y));

								TUnit->GetGazerUnit(IUnit);
							}
						}
						else {
							Manager->UnfoggedData[x + y * Manager->TextureSize] = true;
							currentlyInSight.Add(FVector2D(x, y));
						}
					}
				}
			}
		}
		
	}


	if (Manager->GetIsBlurEnabled()) {
		//Horizontal blur pass
		int offset = floorf(Manager->blurKernelSize / 2.0f);
		for (auto Itr(texelsToBlur.CreateIterator()); Itr; ++Itr) {
			int x = (Itr)->IntPoint().X;
			int y = (Itr)->IntPoint().Y;
			float sum = 0;
			for (int i = 0; i < Manager->blurKernelSize; i++) {
				int shiftedIndex = i - offset;
				if (x + shiftedIndex >= 0 && x + shiftedIndex <= signedSize - 1) {
					if (Manager->UnfoggedData[x + shiftedIndex + (y * signedSize)]) {
						//If we are currently looking at a position, unveil it completely
						if (currentlyInSight.Contains(FVector2D(x + shiftedIndex, y))) {
							sum += (Manager->blurKernel[i] * 255);
						}
						//If this is a previously discovered position that we're not currently looking at, put it into a "shroud of darkness".
						else {
							//sum += (Manager->blurKernel[i] * 100);
							sum += (Manager->blurKernel[i] * Manager->FowMaskColor); //i mod this to make the blurred color unveiled
						}
					}
				}
			}
			Manager->HorizontalBlurData[x + y * signedSize] = (uint8)sum;
		}


		//Vertical blur pass
		for (auto Itr(texelsToBlur.CreateIterator()); Itr; ++Itr) {
			int x = (Itr)->IntPoint().X;
			int y = (Itr)->IntPoint().Y;
			float sum = 0;
			for (int i = 0; i < Manager->blurKernelSize; i++) {
				int shiftedIndex = i - offset;
				if (y + shiftedIndex >= 0 && y + shiftedIndex <= signedSize - 1) {
					sum += (Manager->blurKernel[i] * Manager->HorizontalBlurData[x + (y + shiftedIndex) * signedSize]);
				}
			}
			Manager->TextureData[x + y * signedSize] = FColor((uint8)sum, (uint8)sum, (uint8)sum, 255);
		}
	}
	else {
		for (int y = 0; y < signedSize; y++) {
			for (int x = 0; x < signedSize; x++) {

				if (Manager->UnfoggedData[x + (y * signedSize)]) {
					//If we are currently looking at a position, unveil it completely
					//if the vectors are inside de TSet
					if (currentlyInSight.Contains(FVector2D(x, y))) {
						Manager->TextureData[x + y * signedSize] = FColor(Manager->UnfogColor, Manager->UnfogColor, Manager->UnfogColor, 255);
					}
					//If this is a previously discovered position that we're not currently looking at, put it into a "shroud of darkness".
					else {
						Manager->TextureData[x + y * signedSize] = FColor(Manager->FowMaskColor, Manager->FowMaskColor, Manager->FowMaskColor, 255);
					}
				}


			}
		}
	}
	
	Manager->bHasFOWTextureUpdate = true;
}



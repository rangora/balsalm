// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWarWorker.h"
#include "FogManager.h"
#include "FogRegister.h"
#include "../../../../../Source/RTS_System/Actors/Units/Unit.h"
#include "../../../../../Source/RTS_System/Actors/Units/Component/StatComponent.h"
#include "../../../../../Source/RTS_System/ActorType.h"
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
		if (!Manager->bHasFOWTextureUpdate) {
			UpdateFowTexture();
		}
		FPlatformProcess::Sleep(0.03f);
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
	int textureSize = (int)Manager->TextureSize; 
	int texel_Unit = Manager->TexelUnit;


	TSet<FVector2D> currentlyInSight;
	TSet<FVector2D> texelsToBlur;
	TArray<int> removeIdx;


	// Get fog units.
	for (int idx = 0; idx < Manager->FowActors.Num(); idx++) {
		auto Itr = Manager->FowActors[idx].Get();
		AUnit* IUnit = Cast<AUnit>((Itr));
		FVector position = (IUnit)->GetActorLocation();

		//int textureSize = (int)Manager->TextureSize;
		int sightTexels = IUnit->SightRange;

		// 현실 좌표를 텍셀 좌표로 변환
		int posX = (int)(position.X / texel_Unit) + halfTextureSize;
		int posY = (int)(position.Y / texel_Unit) + halfTextureSize;
		FVector2D UnitTexelSpacePos = FVector2D(posX, posY);

		int halfKernelSize = (Manager->blurKernelSize - 1) / 2; // 7

		

		if (StopTaskCounter.GetValue() != 0) {
			return;
		}

		//Find actor position
		if (!IsValid(IUnit)) {
			removeIdx.Add(idx);
			continue;
		}

		FCollisionQueryParams LayTraceParams(FName(TEXT("fow laytrace Params")), false, (IUnit));
		TArray<FHitResult> Result;


		if (IUnit->UnitStat->DeadOrAlive == DOA::DEAD) {
			removeIdx.Add(idx);

			for (int y = posY - sightTexels; y <= posY + sightTexels; y++) {
				for (int x = posX - sightTexels; x <= posX + sightTexels; x++) {


					//Kernel for radial sight
					if (x > 0 && x < textureSize && y > 0 && y < textureSize) {
						FVector2D currentTextureSpacePos = FVector2D(x, y);
						int length = (int)(UnitTexelSpacePos - currentTextureSpacePos).Size();
						if (length <= sightTexels) {
							FVector currentWorldSpacePos = FVector(
								((x - (int)halfTextureSize)) * texel_Unit,
								((y - (int)halfTextureSize)) * texel_Unit,
								position.Z);

							TArray<AActor*> ActorsToIgnore;
							TArray<AActor*> ActorsToResult;
							TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
							ActorsToIgnore.Add(IUnit);
							ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
							ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));


							if (UKismetSystemLibrary::LineTraceMultiForObjects(Manager->GetWorld(), position, currentWorldSpacePos,
								ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, Result, true)) {

								bool bUnit = false;

								for (FHitResult& TResult : Result) {
									auto TActor = TResult.GetActor();

									if (TActor->IsA(AUnit::StaticClass())) {
										bUnit = true;
										continue;
									}
									else {
										bUnit = false;
										break;
									}
								}

								if (bUnit) {
									Manager->UnfoggedData[x + y * Manager->TextureSize] = true;
									Manager->TextureData[x + y * textureSize] = FColor(Manager->FowMaskColor, Manager->FowMaskColor, Manager->FowMaskColor, 255);
								}

							}
							else {
								Manager->UnfoggedData[x + y * Manager->TextureSize] = true;
								Manager->TextureData[x + y * textureSize] = FColor(Manager->FowMaskColor, Manager->FowMaskColor, Manager->FowMaskColor, 255);
							}
						}
					}
				}
			}

		} // dead end.
		else {

			// 시야안에 들어올 수 있는 텍셀에 대해서만 연산.
			for (int y = posY - sightTexels; y <= posY + sightTexels; y++) {
				for (int x = posX - sightTexels; x <= posX + sightTexels; x++) {


					// 텍셀좌표(x,y)를 경계검사 해준다.
					if (x > 0 && x < textureSize && y > 0 && y < textureSize) {
						FVector2D currentTextureSpacePos = FVector2D(x, y);
						int length = (int)(UnitTexelSpacePos - currentTextureSpacePos).Size();
						// 텍셀이 유닛 시야에 들어오는가.
						if (length <= sightTexels) {
							FVector currentWorldSpacePos = FVector(
								((x - (int)halfTextureSize)) * texel_Unit,
								((y - (int)halfTextureSize)) * texel_Unit,
								position.Z);


							AUnit* TUnit = nullptr;
							TArray<AActor*> ActorsToIgnore;
							TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
							ActorsToIgnore.Add(IUnit);
							ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
							ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));


							if (UKismetSystemLibrary::LineTraceMultiForObjects(Manager->GetWorld(), position, currentWorldSpacePos,
								ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, Result, true)) {

								bool bUnit = false;

								for (FHitResult& TResult : Result) {
									auto TActor = TResult.GetActor();

									if (IsValid(TActor) && TActor->IsA(AUnit::StaticClass())) {
										TUnit = Cast<AUnit>(TActor);
										bUnit = true;
										continue;
									}
									else {
										bUnit = false;
										break;
									}
								}

								// 유닛이면 무시한다.
								if (bUnit) {
									Manager->UnfoggedData[x + y * Manager->TextureSize] = true;
									currentlyInSight.Add(FVector2D(x, y));
									TUnit->GetGazerUnit(IUnit);
								}

							}
							else {
								Manager->UnfoggedData[x + y * Manager->TextureSize] = true;
								// 밝힐 텍셀 좌표를 저장한다.
								currentlyInSight.Add(FVector2D(x, y));
							}
						}
					}
				}
			}

			for (auto Element : currentlyInSight) {
				int x = Element.X;
				int y = Element.Y;
				Manager->TextureData[x + y * textureSize] = FColor(Manager->FowMaskColor,
					Manager->FowMaskColor, Manager->FowMaskColor, 255);
			}

			//Store the positions we want to blur
			for (int y = posY - sightTexels - halfKernelSize; y <= posY + sightTexels + halfKernelSize; y++) {
				for (int x = posX - sightTexels - halfKernelSize; x <= posX + sightTexels + halfKernelSize; x++) {
					if (x > 0 && x < textureSize && y > 0 && y < textureSize) {
						texelsToBlur.Add(FIntPoint(x, y));
					}
				}
			}

			if (Manager->GetIsBlurEnabled()) {
				//Horizontal blur pass
				int offset = floorf(Manager->blurKernelSize / 2.0f);
				for (auto Texel_Itr(texelsToBlur.CreateIterator()); Texel_Itr; ++Texel_Itr) {
					int x = (Texel_Itr)->IntPoint().X;
					int y = (Texel_Itr)->IntPoint().Y;
					float sum = 0;
					for (int i = 0; i < Manager->blurKernelSize; i++) {
						int shiftedIndex = i - offset;
						if (x + shiftedIndex >= 0 && x + shiftedIndex <= textureSize - 1) {
							if (Manager->UnfoggedData[x + shiftedIndex + (y * textureSize)]) {
								//If we are currently looking at a position, unveil it completely
								if (currentlyInSight.Contains(FVector2D(x + shiftedIndex, y))) {
									sum += (Manager->blurKernel[i] * 255);
								}
								else {
									sum += (Manager->blurKernel[i] * Manager->FowMaskColor);	
								}
							}
						}
					}
					Manager->HorizontalBlurData[x + y * textureSize] = (uint8)sum;
				}


				//Vertical blur pass
				for (auto Texel_Itr(texelsToBlur.CreateIterator()); Texel_Itr; ++Texel_Itr) {
					int x = (Texel_Itr)->IntPoint().X;
					int y = (Texel_Itr)->IntPoint().Y;
					float sum = 0;
					for (int i = 0; i < Manager->blurKernelSize; i++) {
						int shiftedIndex = i - offset;
						if (y + shiftedIndex >= 0 && y + shiftedIndex <= textureSize - 1) {
							sum += (Manager->blurKernel[i] * Manager->HorizontalBlurData[x + (y + shiftedIndex) * textureSize]);
						}
					}
					Manager->TextureData[x + y * textureSize] = FColor((uint8)sum, (uint8)sum, (uint8)sum, 255);
				}
			}
		}
	}

	for (auto element : removeIdx) {
		Manager->FowActors.RemoveAt(element);
	}

	Manager->bHasFOWTextureUpdate = true;
}



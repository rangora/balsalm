// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Public/HAL/Runnable.h"


class AFogManager;

class FOGOFWAR_API AFogOfWarWorker : public FRunnable {
public:	
	AFogOfWarWorker();
	AFogOfWarWorker(AFogManager* Manager);
	virtual ~AFogOfWarWorker();
	
	
	//FRunnable interface
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	
	void ShutDown();

	//Method to perform work
	void UpdateFowTexture();

	bool bShouldUpdate = false;
	bool isWriteUnFog = false;
	bool isWriteFow = false;
	bool isWriteTerraIncog = false;

	//Bool, is the actor in terra incognita territory
	bool bCheckActorInTerraIncog = false;

	FRunnableThread* Thread;

	//Pointer to our manager
	AFogManager* Manager;

	//Thread safe counter
	FThreadSafeCounter StopTaskCounter;
};

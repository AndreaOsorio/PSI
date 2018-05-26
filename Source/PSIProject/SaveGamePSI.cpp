// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "SaveGamePSI.h"


USaveGamePSI::USaveGamePSI()
{
	UserIndex = 0;
	CurrentCheckpoint = TEXT("Checkpoint_01");
	AudioVolume = 100.0f;
	EnableRumble = true;
	AudioLogCount = 0;
}




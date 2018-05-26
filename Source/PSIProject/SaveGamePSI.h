// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "SaveGamePSI.generated.h"

/**
 * 
 */
UCLASS()
class PSIPROJECT_API USaveGamePSI : public USaveGame
{
	GENERATED_BODY()
	
public:

	/*
	UPROPERTY(VisibleAnywhere, Category = SaveGame)
		FString PlayerName;
	*/

	USaveGamePSI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
		int AudioLogCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
		int AmmoCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
		int CurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
		FText CurrentObjective;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
		FString CurrentCheckpoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
		int UserIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
		int GammaLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
		bool EnableRumble;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
		float AudioVolume;
};

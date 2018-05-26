// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "PSIProjectGameMode.h"
#include "PoseidonHUD.h"
#include "PoseidonCharacter.h"

APSIProjectGameMode::APSIProjectGameMode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/*Assign the MyCharacter as the default pawn*/
	DefaultPawnClass = APoseidonCharacter::StaticClass();
	HUDClass = APoseidonHUD::StaticClass();
}





// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "PoseidonHUD.generated.h"

enum class EReticleEnum : uint8
{
	RE_AIM,
	RE_HIT_AIM,
	RE_HIP,
	RE_PICKUP, 
	RE_PICKUP_FULL,
	RE_DOOR_OPEN,
	RE_DOOR_CLOSE,
	RE_DOOR_LOCKED,
	RE_ACTIVATE,
	RE_NO_RETICLE
};

UCLASS()
class PSIPROJECT_API APoseidonHUD : public AHUD
{
	GENERATED_BODY()
	
		APoseidonHUD(const FObjectInitializer& ObjectInitializer);

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
#pragma region Crosshairs
	UTexture2D* AimingCrosshairTex;
	UTexture2D* HitAimingCrosshairTex;

	UTexture2D* HipCrosshairTex;

	UTexture2D* PickupCrosshairTex;
	UTexture2D* PickupFullCrosshairTex;

	UTexture2D* DoorOpenCrosshairTex;
	UTexture2D* DoorLockedCrosshairTex;
	UTexture2D* DoorCloseCrosshairTex;

	UTexture2D* ActivateCrosshairTex;

	UTexture2D* CurrentCrosshair;
#pragma endregion

	FVector2D RectCoordinates;

	bool mIsCrosshairEnabled;

	float playerHealth;

	float playerHealthPercentage;

	bool mIsWaterFilling;

public:

	TArray<APawn*> RadarElements;

	void ActivateRadar(TArray<APawn*> PawnsToShow);

	void EnableCrosshair(bool isEnabled);

	void ChangeCrosshair(EReticleEnum CollisionType);

	void SetHUDPlayerHealth(float health, bool waterStarts);

	void DrawCrosshair();
};

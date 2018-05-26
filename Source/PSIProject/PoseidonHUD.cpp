// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "PoseidonHUD.h"
#include "PoseidonCharacter.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"

APoseidonHUD::APoseidonHUD(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> AimingCrosshairTexObj(TEXT("Texture2D'/Game/Art/HUD/Reticles/TXT_Reticle_Aiming.TXT_Reticle_Aiming'"));
	if (AimingCrosshairTexObj.Object)
	{
		AimingCrosshairTex = AimingCrosshairTexObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> HitAimingCrosshairTexObj(TEXT("Texture2D'/Game/Art/HUD/Reticles/TXT_Reticle_AimingRed.TXT_Reticle_AimingRed'"));
	if (HitAimingCrosshairTexObj.Object)
	{
		HitAimingCrosshairTex = HitAimingCrosshairTexObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> HipCrosshairTexObj(TEXT("Texture2D'/Game/Art/HUD/Reticles/TXT_Reticle_Explore.TXT_Reticle_Explore'"));
	if (HipCrosshairTexObj.Object)
	{
		HipCrosshairTex = HipCrosshairTexObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> PickupCrosshairTexObj(TEXT("Texture2D'/Game/Art/HUD/Reticles/TXT_Reticle_Pickup.TXT_Reticle_Pickup'"));
	if (PickupCrosshairTexObj.Object)
	{
		PickupCrosshairTex = PickupCrosshairTexObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> PickupFullCrosshairTexObj(TEXT("Texture2D'/Game/Art/HUD/Reticles/TXT_Reticle_PickupFull.TXT_Reticle_PickupFull'"));
	if (PickupFullCrosshairTexObj.Object)
	{
		PickupFullCrosshairTex = PickupFullCrosshairTexObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> DoorUnlockedCrosshairTexObj(TEXT("Texture2D'/Game/Art/HUD/Reticles/TXT_Reticle_ExploreOpenDoor.TXT_Reticle_ExploreOpenDoor'"));
	if (DoorUnlockedCrosshairTexObj.Object)
	{
		DoorOpenCrosshairTex = DoorUnlockedCrosshairTexObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> DoorCloseCrosshairTexObj(TEXT("Texture2D'/Game/Art/HUD/Reticles/TXT_Reticle_ExploreCloseDoor.TXT_Reticle_ExploreCloseDoor'"));
	if (DoorCloseCrosshairTexObj.Object)
	{
		DoorCloseCrosshairTex = DoorCloseCrosshairTexObj.Object;
	}
																						
	static ConstructorHelpers::FObjectFinder<UTexture2D> DoorLockedCrosshairTexObj(TEXT("Texture2D'/Game/Art/HUD/Reticles/TXT_Reticle_ExploreLockedDoor.TXT_Reticle_ExploreLockedDoor'"));
	if (DoorLockedCrosshairTexObj.Object)
	{
		DoorLockedCrosshairTex = DoorLockedCrosshairTexObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> ActivateCrosshairTexObj(TEXT("Texture2D'/Game/Art/HUD/Reticles/TXT_Reticle_Activate.TXT_Reticle_Activate'"));
	if (ActivateCrosshairTexObj.Object)
	{
		ActivateCrosshairTex = ActivateCrosshairTexObj.Object;
	}
	
	CurrentCrosshair = HipCrosshairTex;
	playerHealth = 100.0f;
	mIsWaterFilling = false;
	playerHealthPercentage = 1.0f;
}

void APoseidonHUD::DrawHUD()
{
	Super::DrawHUD();

	// Find the size of the screen
	FVector2D ScreenSize = FVector2D(Canvas->SizeX, Canvas->SizeY);

	/* CROSSHAIR */
	DrawCrosshair();

	/* HEALTH DAMAGE WATER HUD */
	RectCoordinates.Y = ScreenSize.Y * playerHealthPercentage;
	RectCoordinates.Y = FMath::Clamp(RectCoordinates.Y, 0.0f, ScreenSize.Y);

	//Draw the Rect
	DrawRect(FLinearColor(0.f, 0.f, 1.f, 0.5f), 0.0f, RectCoordinates.Y, ScreenSize.X, ScreenSize.Y);
}

void APoseidonHUD::ActivateRadar(TArray<APawn*> PawnsToShow)
{
	RadarElements = PawnsToShow;
}

void APoseidonHUD::ChangeCrosshair(EReticleEnum CollisionType)
{
	switch (CollisionType)
	{
	case EReticleEnum::RE_AIM:
		CurrentCrosshair = AimingCrosshairTex;
		break;
	case EReticleEnum::RE_HIT_AIM:
		CurrentCrosshair = HitAimingCrosshairTex;
		break;
	case EReticleEnum::RE_HIP:
		CurrentCrosshair = HipCrosshairTex;
		break;
	case EReticleEnum::RE_PICKUP:
		CurrentCrosshair = PickupCrosshairTex;
		break;
	case EReticleEnum::RE_PICKUP_FULL:
		CurrentCrosshair = PickupFullCrosshairTex;
		break;
	case EReticleEnum::RE_DOOR_OPEN:
		CurrentCrosshair = DoorOpenCrosshairTex;
		break;
	case EReticleEnum::RE_DOOR_CLOSE:
		CurrentCrosshair = DoorCloseCrosshairTex;
		break;
	case EReticleEnum::RE_DOOR_LOCKED:
		CurrentCrosshair = DoorLockedCrosshairTex;
		break;
	case EReticleEnum::RE_ACTIVATE:
		CurrentCrosshair = ActivateCrosshairTex;
		break;
	case EReticleEnum::RE_NO_RETICLE:
		CurrentCrosshair = NULL;
		break;
	}
}

void APoseidonHUD::DrawCrosshair()
{
	if (CurrentCrosshair != NULL)
	{
		// Find center of the Canvas
		const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
		// Offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
		const FVector2D CrosshairDrawPosition((Center.X - (CurrentCrosshair->GetSurfaceWidth() * 0.5)),
			(Center.Y - (CurrentCrosshair->GetSurfaceHeight() * 0.5f)));
		// draw the crosshair
		FCanvasTileItem TileItem(CrosshairDrawPosition, CurrentCrosshair->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}

void APoseidonHUD::SetHUDPlayerHealth(float health, bool waterStarts)
{
	playerHealth = health;
	mIsWaterFilling = waterStarts;

	playerHealthPercentage = playerHealth / 100.0f;

	playerHealthPercentage = FMath::Clamp(playerHealthPercentage, 0.0f, 1.0f);
}

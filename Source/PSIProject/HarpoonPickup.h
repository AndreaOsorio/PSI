// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickupObject.h"
#include "HarpoonPickup.generated.h"

/**
*
*/

/**	Type of harpoon ammo enumeration */
UENUM(BlueprintType)
enum class ETypeOfAmmo : uint8
{
	Solo_Ammo  UMETA(DisplayName = "Solo Ammo"),
	Pack_Ammo UMETA(DisplayName = "Pack Ammo"),
};


UCLASS()
class PSIPROJECT_API AHarpoonPickup : public APickupObject
{
	GENERATED_BODY()

	int mPackAmmo;
	int mSoloAmmo;
	int mCurrentAmmo;

	UStaticMesh* SoloMesh;

	UStaticMesh* PackMesh;

public:
	// Sets default values for this actor's properties
	AHarpoonPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
		ETypeOfAmmo TypeOfAmmo;

	virtual void OnConstruction(const FTransform& Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnUsed(AActor* Owner) override;

	virtual void StartFocus() override;

	virtual void EndFocus() override;

	int GetHarpoonAmmo();

	void SetTypeOfAmmo();
};

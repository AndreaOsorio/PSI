// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "HarpoonPickup.h"

// Sets default values
AHarpoonPickup::AHarpoonPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PackMesh_body(TEXT("StaticMesh'/Game/Art/PoseidonHook/HarpoonAmmo.HarpoonAmmo'"));
	if (PackMesh_body.Object)
	{
		PackMesh = PackMesh_body.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SoloMesh_body(TEXT("StaticMesh'/Game/Art/PoseidonHook/Harpoon.Harpoon'"));
	if (SoloMesh_body.Object)
	{
		SoloMesh = SoloMesh_body.Object;
	}

	PickupMesh->SetStaticMesh(SoloMesh);

	mIsBeingUsed = false;

	mSoloAmmo = 1;
	mPackAmmo = 6;
}

void AHarpoonPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetTypeOfAmmo();
}

// Called when the game starts or when spawned
void AHarpoonPickup::BeginPlay()
{
	Super::BeginPlay();
	SetTypeOfAmmo();
	switch (TypeOfAmmo)
	{
	case ETypeOfAmmo::Solo_Ammo:
		PickupMesh->SetSimulatePhysics(true);
		PickupMesh->SetEnableGravity(true);
		PickupMesh->SetConstraintMode(EDOFMode::YZPlane);
		PickupCollider->InitBoxExtent(FVector(5.0f, 70.0f, 5.0f));
		break;
	case ETypeOfAmmo::Pack_Ammo:
		PickupMesh->SetSimulatePhysics(false);
		PickupMesh->SetEnableGravity(false);
		break;
	}
}

// Called every frame
void AHarpoonPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHarpoonPickup::OnUsed(AActor* Owner)
{
	mIsBeingUsed = true;
	PickupMesh->SetVisibility(false);
	Destroy();
}

void AHarpoonPickup::StartFocus()
{
	PickupMesh->SetRenderCustomDepth(true);
	PickupMesh->CustomDepthStencilValue = 252;
}

void AHarpoonPickup::EndFocus()
{
	PickupMesh->SetRenderCustomDepth(false);
}

int AHarpoonPickup::GetHarpoonAmmo()
{
	return mCurrentAmmo;
}

void AHarpoonPickup::SetTypeOfAmmo()
{
	if (TypeOfAmmo == ETypeOfAmmo::Pack_Ammo)
	{
		mCurrentAmmo = mPackAmmo;
		PickupMesh->SetStaticMesh(PackMesh);

	}
	else if (TypeOfAmmo == ETypeOfAmmo::Solo_Ammo)
	{
		mCurrentAmmo = mSoloAmmo;
		PickupMesh->SetStaticMesh(SoloMesh);
	}
}
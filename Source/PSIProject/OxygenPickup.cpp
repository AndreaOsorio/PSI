// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "OxygenPickup.h"

// Sets default values
AOxygenPickup::AOxygenPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PickupMesh_body(TEXT("StaticMesh'/Game/Art/EnviromentArt/OxygenPickUp/MESH_PickupOxygen.MESH_PickupOxygen'"));
	if (PickupMesh_body.Object)
	{
		PickupMesh->SetStaticMesh(PickupMesh_body.Object);
	}

	mIsBeingUsed = false;
}

// Called when the game starts or when spawned
void AOxygenPickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOxygenPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOxygenPickup::OnUsed(AActor* Owner)
{
	mIsBeingUsed = true;
	PickupMesh->SetVisibility(false);
	Destroy();
}

void AOxygenPickup::StartFocus()
{
	PickupMesh->SetRenderCustomDepth(true);
	PickupMesh->CustomDepthStencilValue = 252;
}

void AOxygenPickup::EndFocus()
{
	PickupMesh->SetRenderCustomDepth(false);
}
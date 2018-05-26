// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "PoseidonCharacter.h"
#include "ObjectivePoint.h"

AObjectivePoint::AObjectivePoint()
{

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	Collider->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	Collider->AttachTo(RootComponent);
	Collider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ObjectiveRadarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DecalMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube_obj(TEXT("StaticMesh'/Game/Art/HUD/Radar/MESH_PlayerIconRadar.MESH_PlayerIconRadar'"));
	if (Cube_obj.Object)
	{
		ObjectiveRadarMesh->SetStaticMesh(Cube_obj.Object);
	}

	ObjectiveRadarMesh->AttachTo(RootComponent);
	ObjectiveRadarMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2160.0f));
	ObjectiveRadarMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

}

void AObjectivePoint::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AObjectivePoint::OnOverlapBegin);
}

void AObjectivePoint::OnOverlapBegin(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	APoseidonCharacter* Player = Cast<APoseidonCharacter>(OtherActor);
	if (Player)
	{
		Player->ClearObjective();
		this->Destroy();
	}
	
}
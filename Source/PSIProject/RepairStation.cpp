// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "PoseidonCharacter.h"
#include "RepairStation.h"
#include "Engine.h"

//
// Sets default values
ARepairStation::ARepairStation()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Repair Station mesh
	RepairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RepairStation_obj(TEXT("StaticMesh'/Game/Art/EnviromentArt/RepairStation/MESH_RepairStation.MESH_RepairStation'"));
	if (RepairStation_obj.Object)
	{
		RepairMesh->SetStaticMesh(RepairStation_obj.Object);
		RepairMesh->bLightAsIfStatic = 1;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RepairStationEmpty_obj(TEXT("StaticMesh'/Game/Art/EnviromentArt/RepairStation/MESH_RepairStationEmpty.MESH_RepairStationEmpty'"));
	if (RepairStationEmpty_obj.Object)
	{
		RepairMeshEmpty = RepairStationEmpty_obj.Object;
	}
	
	
	// Box Collider
	RepairCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RepairCollider->InitBoxExtent(FVector(100.0f, 150.0f, 100.0f));
	RepairCollider->AttachTo(RepairMesh);
	RepairCollider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	isPlayerInteracting = false;
	isPlayerInside = false;
	mWasUsed = false;
}

// Called when the game starts or when spawned
void ARepairStation::BeginPlay()
{
	Super::BeginPlay();
	//Cone start On Overlap functions
	if (RepairCollider)
	{
		RepairCollider->OnComponentBeginOverlap.AddDynamic(this, &ARepairStation::OnOverlapBegin);
		RepairCollider->OnComponentEndOverlap.AddDynamic(this, &ARepairStation::OnOverlapEnd);
	}
}

// Called every frame
void ARepairStation::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (isPlayerInside && Player != nullptr)
	{
		
		isPlayerInteracting = Player->GetIsInteracting();
		if (isPlayerInteracting)
		{
			if (!mWasUsed)
			{
				Player->SetIsInteracting(false);
				Player->RepairPlayer();
				RepairMesh->SetStaticMesh(RepairMeshEmpty);
				mWasUsed = true;
				RepairMesh->CustomDepthStencilValue = 255;

			}
		}
	}
}

// Called to bind functionality to input
void ARepairStation::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ARepairStation::OnOverlapBegin(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Player = Cast<APoseidonCharacter>(OtherActor);
	if (Player)
	{
		RepairMesh->SetRenderCustomDepth(true);
		if (mWasUsed)
		{
			RepairMesh->CustomDepthStencilValue = 255;
		}
		else
		{
			RepairMesh->CustomDepthStencilValue = 252;
		}
		
		isPlayerInside = true;
	}
	
}

void ARepairStation::OnOverlapEnd(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Player = Cast<APoseidonCharacter>(OtherActor);
	if (Player)
	{
		RepairMesh->SetRenderCustomDepth(false);
		isPlayerInside = false;
	}
}

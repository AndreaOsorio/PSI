// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "PickupObject.h"


// Sets default values
APickupObject::APickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PickupMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RootComponent = PickupMesh;
	
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleObj(TEXT("ParticleSystem'/Game/Art/ParticleSystem/Bubble/PAR_PipeBubbles.PAR_PipeBubbles'"));
	if (ParticleObj.Object) {
		ParticleComponent->Template = ParticleObj.Object;
		ParticleComponent->AttachTo(RootComponent);
	}

	PickupCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	PickupCollider->AttachTo(RootComponent);
	PickupCollider->InitBoxExtent(FVector(10.0f, 10.0f, 5.0f));
	PickupCollider->SetCollisionProfileName(TEXT("BlockAll"));
	PickupCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	PickupCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	mIsBeingUsed = false;
}

// Called when the game starts or when spawned
void APickupObject::BeginPlay()
{
	Super::BeginPlay();
	PickupMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	PickupCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
}

// Called every frame
void APickupObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickupObject::OnUsed(AActor * Owner)
{
	mIsBeingUsed = true;
}

void APickupObject::StartFocus()
{
	PickupMesh->SetRenderCustomDepth(true);
	PickupMesh->CustomDepthStencilValue = 252;
}

void APickupObject::EndFocus()
{
	PickupMesh->SetRenderCustomDepth(false);
}

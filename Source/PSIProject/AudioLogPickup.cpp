// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "AudioLogPickup.h"

// Sets default values
AAudioLogPickup::AAudioLogPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PickupMesh_body(TEXT("StaticMesh'/Game/Art/EnviromentArt/AudioLog/MESH_AudioLog.MESH_AudioLog'"));
	if (PickupMesh_body.Object)
	{
		PickupMesh->SetStaticMesh(PickupMesh_body.Object);
	}

	mIsBeingUsed = false;
}

// Called when the game starts or when spawned
void AAudioLogPickup::BeginPlay()
{
	Super::BeginPlay();
	PickupCollider->InitBoxExtent(FVector(20.0f, 30.0f, 10.0f));
	PickupCollider->SetRelativeLocation(FVector(-5.0f, 0.0f, 0.0f));
}

// Called every frame
void AAudioLogPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAudioLogPickup::OnUsed(AActor* Owner)
{
	PickupMesh->SetVisibility(false);
	Destroy();
}

void AAudioLogPickup::StartFocus()
{
	PickupMesh->SetRenderCustomDepth(true);
	PickupMesh->CustomDepthStencilValue = 252;
}

void AAudioLogPickup::EndFocus()
{
	PickupMesh->SetRenderCustomDepth(false);
}

void AAudioLogPickup::PlayAudio(int audioLogNumber, AActor* Owner)
{
	if (Owner)
	{
		Player = Owner;
		switch (audioLogNumber)
		{
		case 0:
			PlayWwiseEvent("Log1");
			break;
		case 1:
			PlayWwiseEvent("Log2");
			break;
		case 2:
			PlayWwiseEvent("Log3");
			break;
		case 3:
			PlayWwiseEvent("Log4");
			break;
		case 4:
			PlayWwiseEvent("Log5");
			break;
		case 5:
			PlayWwiseEvent("Log6");
			break;
		case 6:
			PlayWwiseEvent("Log7");
			break;
		case 7:
			PlayWwiseEvent("Log8");
			break;
		case 8:
			PlayWwiseEvent("Log9");
			break;
		case 9:
			PlayWwiseEvent("Log10");
			break;
		}
	}
}

void AAudioLogPickup::PlayWwiseEvent(FString audioTape)
{
	if (!mIsBeingUsed)
	{
		mIsBeingUsed = true;
		//Reproduce Audio (Wwise event) here
		FString audioEvent = "Event_CorLog";
		FString audioPerson = "Coraline";
		AAudioManager::GetInstance()->PlayWwiseEventWithSwitch(audioEvent, Player, audioPerson, audioTape);
	}
}
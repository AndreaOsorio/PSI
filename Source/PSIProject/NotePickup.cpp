// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "UIManager.h"
#include "NotePickup.h"

// Sets default values
ANotePickup::ANotePickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PickupMesh_body(TEXT("StaticMesh'/Game/Art/EnviromentArt/NotesPickUp/MESH_PaperHolderClose.MESH_PaperHolderClose'"));
	if (PickupMesh_body.Object)
	{
		PickupMesh->SetStaticMesh(PickupMesh_body.Object);
	}
	mNoteNumber = 0;

	mIsBeingUsed = false;
}

// Called when the game starts or when spawned
void ANotePickup::BeginPlay()
{
	Super::BeginPlay();
	
	switch (NoteNumber)
	{
	case ENoteNumber::Note_1:
		mNoteNumber = 0;
		break;
	case ENoteNumber::Note_2:
		mNoteNumber = 1;
		break;
	case ENoteNumber::Note_3:
		mNoteNumber = 2;
		break;
	case ENoteNumber::Note_4:
		mNoteNumber = 3;
		break;
	case ENoteNumber::Note_5:
		mNoteNumber = 4;
		break;
	case ENoteNumber::Note_6:
		mNoteNumber = 5;
		break;
	case ENoteNumber::Note_7:
		mNoteNumber = 6;
		break;
	case ENoteNumber::Note_8:
		mNoteNumber = 7;
		break;
	case ENoteNumber::Note_9:
		mNoteNumber = 8;
		break;
	case ENoteNumber::Note_10:
		mNoteNumber = 9;
		break;
	case ENoteNumber::Note_11:
		mNoteNumber = 10;
		break;
	case ENoteNumber::Note_12:
		mNoteNumber = 11;
		break;
	case ENoteNumber::Note_13:
		mNoteNumber = 12;
		break;
	case ENoteNumber::Note_14:
		mNoteNumber = 13;
		break;
	case ENoteNumber::Note_15:
		mNoteNumber = 14;
		break;
	case ENoteNumber::Note_16:
		mNoteNumber = 15;
		break;
	case ENoteNumber::Note_17:
		mNoteNumber = 16;
		break;
	}
}

void ANotePickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called every frame
void ANotePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANotePickup::OnUsed(AActor* Owner)
{
	if (!mIsBeingUsed)
	{
		for (TActorIterator<AUIManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AUIManager* UI = Cast<AUIManager>(*ActorItr);
			if (UI)
			{
				FOutputDeviceNull ar;
				FString FunctionName = FString::Printf(TEXT("SetNoteNumber %i"), mNoteNumber);
				UI->CallFunctionByNameWithArguments(*FunctionName, ar, NULL, true);
				//UI->CallFunctionByNameWithArguments(TEXT("SetNoteNumber 2"), ar, NULL, true);
				UI->ShowNote();
			}
		}
	}
	mIsBeingUsed = true;
}

void ANotePickup::StartFocus()
{
	PickupMesh->SetRenderCustomDepth(true);
	PickupMesh->CustomDepthStencilValue = 252;
}

void ANotePickup::EndFocus()
{
	PickupMesh->SetRenderCustomDepth(false);
}

void ANotePickup::SetUsed()
{
	mIsBeingUsed = false;
}
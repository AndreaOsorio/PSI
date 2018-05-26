// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PoseidonCharacter.h"
#include "AudioManager.h"
#include "RepairStation.generated.h"

UCLASS()
	//
class PSIPROJECT_API ARepairStation : public APawn
{
	GENERATED_BODY()

	//Repair Station mesh
	UStaticMeshComponent* RepairMesh;

	UStaticMesh* RepairMeshEmpty;

	// Box Collider
	UBoxComponent* RepairCollider;

	APoseidonCharacter* Player;

	bool isPlayerInside;
	bool isPlayerInteracting;
	bool mWasUsed;
public:
	// Sets default values for this pawn's properties
	ARepairStation();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION()
		virtual void OnOverlapBegin(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
